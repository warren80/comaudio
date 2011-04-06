#include <QFileDialog>
#include <QFileSystemModel>
#include <QObjectList>

#ifndef _WIN32
#include <sys/socket.h>
#endif

#ifdef _WIN32
#include <winsock2.h>
#endif

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "serverstream.h"
#include "componentvoice.h"

/******************************************
 * ESSENTIAL METHODS
 ******************************************/

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow), appServer_(0), appClient_(0), mic_(0), micThread_(0), stream_(0), streamServer_(0), streamThread_(0), receivedFile_(0), localPlayer_(0)
{
    setWindowIcon(QIcon(":/kidnapster.png"));
    QValidator *validIp = new QRegExpValidator(QRegExp("^\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}$"), this);

    ui->setupUi(this);

    //Setting regex validation
    ui->serverAddrBox->setValidator(validIp);

    ui->localSongList->addItems(QDir("./Songs").entryList(QStringList("*.wav")));

    /**
    * CONNECTIONS
    */

    //Audio Player
    connect(ui->play, SIGNAL(pressed()), this, SLOT(playSong()));
    connect(ui->refreshLocalSongList, SIGNAL(pressed()), this, SLOT(refreshLocalList()));
    connect(ui->stop, SIGNAL(pressed()), this, SLOT(stopSong()));

    //Client Tab
    connect(ui->connectButton, SIGNAL(pressed()), this, SLOT(appConnectClient()));
    connect(ui->disconnectButton, SIGNAL(pressed()), this, SLOT(appDisconnectClient()));

    //Files Tab
    connect(ui->refreshServerFiles, SIGNAL(pressed()), this, SLOT(refreshServerFilesList()));

    //Voice Tab
    connect(ui->startTalkingButton, SIGNAL(pressed()), this, SLOT(startVoice()));
    connect(ui->stopTalkingButton, SIGNAL(pressed()), this, SLOT(stopVoice()));

    //Server Tab
    connect(ui->broadcastButton, SIGNAL(pressed()), this, SLOT(broadcastSong()));
    connect(ui->refreshSongs, SIGNAL(pressed()), this, SLOT(refreshSongList()));
    connect(ui->startServerButton, SIGNAL(pressed()), this, SLOT(appStartServer()));
    connect(ui->stopServerButton, SIGNAL(pressed()), this, SLOT(appStopServer()));

    notes_.setFileName(":/notes.gif");
    cylon_.setFileName(":/cylon.gif");
    waiting_.setFileName(":/waiting.gif");
    ui->cylon->setMovie(&cylon_);
    ui->notes->setMovie(&notes_);
    ui->waiting->setMovie(&waiting_);

    // stream (radio)
    streamThread_ = new Thread();
    streamServer_ = new ServerStream();
    connect(this, SIGNAL(playThisSong(QString)), streamServer_, SLOT(slotStartTransfer(QString)));
    connect(streamServer_, SIGNAL(signalTransferDone()), streamThread_, SLOT(deleteLater()));
    connect(this, SIGNAL(stopThisSong()), streamServer_, SLOT(slotCleanup()));
    streamServer_->moveToThread(streamThread_);
    streamThread_->start();


    // file transmits
    connect(ui->downloadCurrentSongButton, SIGNAL(clicked()), this, SLOT(slotStartTransmitCurrent()));
    connect(ui->downloadSongButton, SIGNAL(clicked()), this, SLOT(slotStartTransmitSelected()));
    //printDebug(QString("asdf"));
}

MainWindow::~MainWindow() {
    if (mic_ != 0) {
        delete mic_;
    }
    if (micThread_ != 0) {
        delete micThread_;
    }
    if (stream_ != 0) {
        delete stream_;
    }
    if (streamServer_ != 0) {
        delete streamServer_;
    }
    if (streamThread_ != 0) {
        streamThread_->terminate();
        delete streamThread_;
    }
    if (receivedFile_ != 0) {
        delete receivedFile_;
    }
    if (appServer_ != 0) {
        delete appServer_;
    }
    if (appClient_ != 0) {
        delete appClient_;
    }
    if (localPlayer_ != 0) {
        delete localPlayer_;
    }
    delete ui;
}

void MainWindow::initDispatcher() {
    Thread * dispatcherThread = new Thread();
    dispatcherThread->start();

}

/******************************************
 * HELPERS
 ******************************************/

void MainWindow::clientConnect(bool connected) {
    ui->connectButton->setEnabled(!connected);
    ui->disconnectButton->setEnabled(connected);
    ui->fileTab->setEnabled(connected);
    ui->startTalkingButton->setEnabled(connected);
    ui->stopTalkingButton->setEnabled(!connected);

    if(!connected) {
        setWindowTitle("Kidnapster - Disconnected");
        cylon_.stop();
        notes_.stop();
    } else {
        cylon_.start();
        notes_.start();
    }
}

void MainWindow::serverConnect(bool connected) {
    ui->startServerButton->setEnabled(!connected);
    ui->broadcastButton->setEnabled(connected);
    ui->stopServerButton->setEnabled(connected);
    ui->refreshSongs->setEnabled(connected);
    ui->songList->setEnabled(connected);

    if(!connected) {
        setWindowTitle("Kidnapster - Disconnected");
        cylon_.stop();
        notes_.stop();
    } else {
        cylon_.start();
    }
}

void MainWindow::printF(QString str) {
    //ui->debug->clear();
    ui->debug->appendPlainText(str.toLatin1().data());
}

/******************************************
 * SLOTS
 ******************************************/

void MainWindow::appConnectClient() {
    QString ipAddr(ui->serverAddrBox->text());

    try {
        appClient_ = new Client();
    } catch (const QString& e) {
        printF(e);
    }


    if (!appClient_->connect(inet_addr(ipAddr.toStdString().c_str()), htons(8001))) {
        delete appClient_;
        appClient_ = 0;
        return;
    }

    stream_ = new ComponentStream();
    setWindowTitle("Kidnapster - Client");
    connect(appClient_, SIGNAL(signalStopStream()), this, SLOT(slotStopStream()));
    connect(appClient_, SIGNAL(signalFileListReceived(char*,int)), this, SLOT(slotReceiveFileList(char*,int)));
    connect(appClient_, SIGNAL(signalShutdown()), this, SLOT(appDisconnectClient()));
    connect(appClient_, SIGNAL(signalSongName(QString)), this, SLOT(slotClientSongName(QString)));
    connect(stream_, SIGNAL(signalSongData(WaveHeader*)), this, SLOT(slotClientSongInfo(WaveHeader*)));
    connect(appClient_, SIGNAL(signalFileFinished()), this, SLOT(slotFinishTransmit()));
    connect(appClient_, SIGNAL(signalFileDataReceived(char*,int)), this, SLOT(slotReceiveTransmitData(char*,int)));
    connect(appClient_, SIGNAL(signalPrintF(QString)), this, SLOT(printF(QString)));

    appClient_->start();
    stream_->start();

    printF(QString("Connected to Server"));
    printF(QString("Joining Multicast Session"));

    cylon_.start();
    clientConnect(true);
    ui->tabWidget->setTabEnabled(2, false);
    ui->serverAddrBox->setEnabled(false);
}

void MainWindow::appDisconnectClient() {
    appClient_->disconnect();
    stream_->disconnect();
    stream_->terminate();

    slotClientSongName(QString());
    stream_->wait();

    delete appClient_;
    appClient_ = 0;
    delete stream_;
    stream_ = 0;
    ui->serverFilesView->clear();
    clientConnect(false);
    ui->tabWidget->setTabEnabled(2, true);
    ui->serverAddrBox->setEnabled(true);
    ui->stopTalkingButton->setEnabled(false);
    printF(QString("Disconnected from Server"));
}

void MainWindow::appStartServer() {
    setWindowTitle("Kidnapster - Server");
    try {
        appServer_ = new Server(htons(8001));
    } catch (const QString& e) {
        printF(QString(e));
    }
    connect(this, SIGNAL(stopThisSong()), appServer_, SLOT(slotDisconnectStream()));
    connect(appServer_, SIGNAL(signalClientConnect(Socket*)), this, SLOT(slotSendFileList(Socket*)));
    connect(appServer_, SIGNAL(signalClientConnect(Socket*)), this, SLOT(slotSendFileName(Socket*)));
    connect(this, SIGNAL(playThisSong(QString)), appServer_, SLOT(slotPlayThisSong(QString)));
    connect(appServer_, SIGNAL(signalPrintF(QString)), this, SLOT(printF(QString)));

    appServer_->start();
    refreshSongList();
    serverConnect(true);
    ui->tabWidget->setTabEnabled(0, false);
    ui->tabWidget->setTabEnabled(1, false);
}

void MainWindow::appStopServer() {
    appServer_->disconnect();
    delete appServer_;
    appServer_ = 0;
    ui->songList->clear();
    serverConnect(false);
    emit stopThisSong();
    ui->broadcastButton->setText("Broadcast");
    ui->tabWidget->setTabEnabled(0, true);
    ui->tabWidget->setTabEnabled(1, true);
    ui->fileTab->setEnabled(false);
}

void MainWindow::broadcastSong() {
    if(ui->broadcastButton->text() == "Broadcast") {
        QString songName = ui->songList->currentItem()->text();
        notes_.start();
        ui->currentSong->setText(songName);
        ui->broadcastButton->setText("Stop Song");
        emit playThisSong(QDir::currentPath() + "/Songs/" + songName);
    } else {
        notes_.stop();
        ui->broadcastButton->setText("Broadcast");
        ui->currentSong->clear();
        emit stopThisSong();
    }
}

void MainWindow::slotStopStream() {
    stream_->resetAudio();
}


void MainWindow::slotSendFileList(Socket* socket) {
    QAbstractItemModel* model = ui->songList->model();
    QStringList strings;
    int size = 0;
    for (int i = 0; i < model->rowCount(); ++i) {
      strings << model->index(i, 0).data(Qt::DisplayRole).toString();
      size += model->index( i, 0 ).data( Qt::DisplayRole ).toString().length() + 1;
    }

    Packet packet;
    packet.length = size;
    packet.type = kFileList;

    QString data;

    for (int i = 0; i < strings.size(); i++) {
        data.append(strings[i]).append('|');
    }

    data.chop(1);

    packet.data = (char*) data.toStdString().c_str();

    socket->transmit(packet);
}

void MainWindow::slotReceiveFileList(char *data, int length) {
    QStringList list(QString(data).split('|'));

    ui->serverFilesView->clear();
    ui->serverFilesView->addItems(list);
    ui->serverFilesView->setCurrentItem(ui->serverFilesView->item(0));

    delete[] data;

}

void MainWindow::refreshServerFilesList() {
    slotSendFileList(appClient_->getSocket());
}

void MainWindow::refreshSongList() {
    QDir dir("./Songs");
    QStringList files = dir.entryList(QStringList("*.wav"));

    ui->songList->clear();
    ui->songList->addItems(files);
    ui->songList->setCurrentItem(ui->songList->item(0));
}

void MainWindow::startVoice() {
    printF(QString("Starting Voice session with server"));
    Packet pckt;
    pckt.data = new char;
    *pckt.data = 1;
    pckt.length = 1;
    pckt.type = kVoice;

    appClient_->getSocket()->transmit(pckt);
    Thread *thread = new Thread();
    ComponentVoice *cv = 0;
    try {
        cv = new ComponentVoice(appClient_->getSocket());
    } catch (const QString& e) {
        printF(e);
        thread->exit();
        thread->wait();
        delete thread;
        delete cv;
        ui->startTalkingButton->setEnabled(true);
        ui->stopTalkingButton->setEnabled(false);
        return;
    }
    thread->start();
    cv->moveToThread(thread);
    QObject::connect(this, SIGNAL(signalStopVoiceComponent()),cv, SLOT(slotStopVoiceComponent()));
    QObject::connect(appClient_,SIGNAL(signalVoiceMessage(char*,int)), cv,SLOT(receiveData(char*,int)));
    QObject::connect(this,SIGNAL(signalStartComponentVoice()),cv,SLOT(slotStartComponentVoice()));
    emit signalStartComponentVoice();
    ui->startTalkingButton->setEnabled(false);
    ui->stopTalkingButton->setEnabled(true);
}

void MainWindow::stopVoice() {
    printF("Ending voice session with server");
    Packet pckt;
    pckt.data = new char;
    pckt.length = 1;
    *pckt.data = 0;
    pckt.type = kVoice;
    emit signalStopVoiceComponent();
    appClient_->getSocket()->transmit(pckt);
    ui->startTalkingButton->setEnabled(true);
    ui->stopTalkingButton->setEnabled(false);

}

void MainWindow::slotClientSongName(QString songname) {
    ui->currentSongText->setText(songname);
    ui->songNameText->setText(songname);
    if (songname.size() == 0) {
        slotClientSongInfo(NULL);
    }
}

void MainWindow::slotClientSongInfo(WaveHeader* header) {
    if (header == 0) {
        ui->sampleRateText->clear();
        ui->sampleSizeText->clear();
        ui->channelsText->clear();
        return;
    }
    ui->sampleRateText->setText(QString().setNum(header->frequency));
    ui->sampleSizeText->setText(QString().setNum(header->bitsPerSample));
    ui->channelsText->setText(QString().setNum(header->channels));
    delete header;
}

void MainWindow::slotStartTransmitCurrent() {
    if(ui->waiting->isHidden()) {
        ui->waiting->show();
    }
    waiting_.start();
    ui->downloadCurrentSongButton->setEnabled(false);
    ui->downloadSongButton->setEnabled(false);
    slotStartTransmit(ui->currentSongText->text());
}

void MainWindow::slotStartTransmitSelected() {
    if (ui->serverFilesView->selectedItems().count() == 0) {
        return;
    }

    if(ui->waiting->isHidden()) {
        ui->waiting->show();
    }
    waiting_.start();
    ui->downloadCurrentSongButton->setEnabled(false);
    ui->downloadSongButton->setEnabled(false);
    slotStartTransmit(ui->serverFilesView->selectedItems().first()->text());
}

void MainWindow::slotStartTransmit(QString filename) {

    if (!QDir("Songs").exists()) {
        QDir().mkdir("Songs");
    }
    receivedFile_ = new QFile("./Songs/" + filename);
    receivedFile_->open(QFile::WriteOnly);
    Packet packet;
    packet.length = filename.size() + 1;
    packet.type = kTransfer;
    packet.data = (char*) filename.toStdString().c_str();
    appClient_->getSocket()->transmit(packet);
}

void MainWindow::slotReceiveTransmitData(char *data, int length) {

    receivedFile_->write(data, length);

    delete[] data;
}

void MainWindow::slotFinishTransmit() {
    waiting_.stop();
    ui->downloadCurrentSongButton->setEnabled(true);
    ui->downloadSongButton->setEnabled(true);
    ui->waiting->hide();
    receivedFile_->close();
    delete receivedFile_;
    receivedFile_ = 0;
}

void MainWindow::slotSendFileName(Socket *socket) {
    Packet packet;
    packet.length = ui->currentSong->text().size() + 1;
    packet.data = (char*) ui->currentSong->text().toStdString().c_str();
    packet.type = kStream;
    socket->transmit(packet);
}

void MainWindow::playSong() {
    static bool playing = false;
    if(!playing) {
        QString song = ui->localSongList->currentText();

        if(song == "") {
            return;
        }

        ui->play->setStyleSheet("QPushButton {background-image: url(:/pause.gif);background-repeat: no-repeat;background-position: center;background-color: rgba(255,255,255,0%);}QPushButton:hover {background-image: url(:/pauseHover.gif);}QPushButton:pressed {background-image: url(:/pausePress.gif);}");
        ui->songName->setStyleSheet("color: blue;");
        ui->songName->setText(song);

        if (localPlayer_ == 0) {
            QFile localFile(QDir::currentPath() + "/Songs/" + song);
            localFile.open(QFile::ReadOnly);

            WaveHeader* header = AudioPlayer::parseWaveHeader(localFile.read(44).data());

            localPlayer_ = new AudioPlayer(header->frequency, header->channels, header->bitsPerSample, localFile.size());
            localPlayer_->appendBuffer(localFile.readAll().data(), localFile.size() - 44);
        }

        localPlayer_->play();

        playing = true;
    } else {
        ui->play->setStyleSheet("QPushButton {background-image: url(:/play.gif);background-repeat: no-repeat;background-position: center;background-color: rgba(255,255,255,0%);}QPushButton:hover {background-image: url(:/playHover.gif);}QPushButton:pressed {background-image: url(:/playPress.gif);}");
        ui->songName->setStyleSheet("color: red;");

        localPlayer_->pause();

        playing = false;
    }
}

void MainWindow::stopSong() {
    if (localPlayer_ != 0) {
        playSong();
        delete localPlayer_;
        localPlayer_ = 0;
    }
}

void MainWindow::refreshLocalList() {
    ui->localSongList->clear();
    ui->localSongList->addItems(QDir("./Songs").entryList(QStringList("*.wav")));
}

void MainWindow::print(const char * str) {
    signalPrintF(str);
}
