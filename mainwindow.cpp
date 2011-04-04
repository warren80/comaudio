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
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    setWindowIcon(QIcon(":/kidnapster.png"));
    //ComponentVoice *cv;
    QValidator *validIp = new QRegExpValidator(QRegExp("^\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}$"), this);

    ui->setupUi(this);

    //Setting regex validation
    ui->serverAddrBox->setValidator(validIp);

    /**
    * CONNECTIONS
    */

    //Client Tab
    connect(ui->connectButton, SIGNAL(pressed()), this, SLOT(appConnectClient()));
    connect(ui->disconnectButton, SIGNAL(pressed()), this, SLOT(appDisconnectClient()));

    //Files Tab
    connect(ui->downloadSongButton, SIGNAL(pressed()), this, SLOT(downloadSong()));
    connect(ui->downloadCurrentSongButton, SIGNAL(pressed()), this, SLOT(downloadCurrentSong()));

    //Voice Tab
    connect(ui->startTalkingButton, SIGNAL(pressed()), this, SLOT(startVoice()));
    connect(ui->stopTalkingButton, SIGNAL(pressed()), this, SLOT(stopVoice()));

    //Server Tab
    connect(ui->broadcastButton, SIGNAL(pressed()), this, SLOT(broadcastSong()));
    connect(ui->refreshSongs, SIGNAL(pressed()), this, SLOT(refreshSongList()));
    connect(ui->startServerButton, SIGNAL(pressed()), this, SLOT(appStartServer()));
    connect(ui->stopServerButton, SIGNAL(pressed()), this, SLOT(appStopServer()));

    //Audio Player
    connect(ui->playButton, SIGNAL(pressed()), this, SLOT(playSong()));
    connect(ui->pauseButton, SIGNAL(pressed()), this, SLOT(pauseSong()));

    notes_.setFileName(":/notes.gif");
    cylon_.setFileName(":/cylon.gif");
    ui->cylon->setMovie(&cylon_);
    ui->notes->setMovie(&notes_);


    streamThread_ = new Thread();
    streamServer_ = new ServerStream();
    connect(this, SIGNAL(playThisSong(QString)), streamServer_, SLOT(slotStartTransfer(QString)));
    connect(streamServer_, SIGNAL(signalTransferDone()), streamThread_, SLOT(deleteLater()));
    connect(this, SIGNAL(stopThisSong()), streamServer_, SLOT(slotCleanup()));
    streamServer_->moveToThread(streamThread_);
    streamThread_->start();
}

MainWindow::~MainWindow() {
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
    ui->stopTalkingButton->setEnabled(connected);
    ui->playButton->setEnabled(connected);
    ui->pauseButton->setEnabled(connected);

    if(!connected) {
        setWindowTitle("Kidnapster - Disconnected");
        cylon_.stop();
        notes_.stop();
    } else {
        cylon_.start();
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

/******************************************
 * SLOTS
 ******************************************/

void MainWindow::appConnectClient() {
    QString ipAddr(ui->serverAddrBox->text());

    try {
        appClient_ = new Client();
    } catch (const QString& e) {
        qDebug() << e;
    }

    if (!appClient_->connect(inet_addr(ipAddr.toStdString().c_str()), htons(8001))) {
        delete appClient_;
        return;
    }

    stream_ = new ComponentStream();
    setWindowTitle("Kidnapster - Client");
    connect(stream_, SIGNAL(signalReceivedData(int)), this, SLOT(rate(int)));
    connect(appClient_, SIGNAL(signalStopStream()), this, SLOT(slotStopStream()));
    connect(appClient_, SIGNAL(signalFileListReceived(char*,int)), this, SLOT(slotReceiveFileList(char*,int)));
    connect(appClient_, SIGNAL(signalShutdown()), this, SLOT(appDisconnectClient()));
    connect(appClient_, SIGNAL(signalSongName(QString)), this, SLOT(slotClientSongName(QString)));
    connect(stream_, SIGNAL(signalSongData(WaveHeader*)), this, SLOT(slotClientSongInfo(WaveHeader*)));

    appClient_->start();
    stream_->start();

    cylon_.start();
    clientConnect(true);
    ui->tabWidget->setTabEnabled(2, false);
}

void MainWindow::appDisconnectClient() {
    appClient_->disconnect();
    stream_->disconnect();
    stream_->terminate();
    stream_->wait();

    delete appClient_;
    delete stream_;
    ui->playButton->setText("Tune In");
    ui->serverFilesView->clear();
    clientConnect(false);
    ui->tabWidget->setTabEnabled(2, true);
}

void MainWindow::appStartServer() {
    setWindowTitle("Kidnapster - Server");
    try {
        appServer_ = new Server(htons(8001));
    } catch (const QString& e) {
        qDebug() << e;
    }
    connect(this, SIGNAL(stopThisSong()), appServer_, SLOT(slotDisconnectStream()));
    connect(appServer_, SIGNAL(signalSendFileList(Socket*)), this, SLOT(slotSendFileList(Socket*)));
    connect(this, SIGNAL(playThisSong(QString)), appServer_, SLOT(slotPlayThisSong(QString)));

    appServer_->start();
    refreshSongList();
    serverConnect(true);
    ui->tabWidget->setTabEnabled(0, false);
    ui->tabWidget->setTabEnabled(1, false);
}

void MainWindow::appStopServer() {
    appServer_->disconnect();
    delete appServer_;
    ui->songList->clear();
    serverConnect(false);
    emit stopThisSong();
    ui->broadcastButton->setText("Broadcast");
    ui->tabWidget->setTabEnabled(0, true);
    ui->tabWidget->setTabEnabled(1, true);
}

/*
* TODO:
* 1. Send request
*/
void MainWindow::downloadSong() {
    QString songName = ui->serverFilesView->currentItem()->text();

}

/*
* TODO:
* 1. Send Request
*/
void MainWindow::downloadCurrentSong() {
    QString songName = ui->currentSongText->text();

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

    delete[] data;

}

void MainWindow::refreshSongList() {
    QDir dir("./Songs");
    QStringList files = dir.entryList(QStringList("*.wav"));

    ui->songList->clear();
    ui->songList->addItems(files);
}

void MainWindow::startVoice() {
    ComponentVoice *cv = 0;
    Packet pckt;
    pckt.data = 0;
    pckt.length = 0;
    pckt.type = kVoice;
    Thread *thread = new Thread();

    appClient_->getSocket()->transmit(pckt);

    try {
        cv = new ComponentVoice(appClient_->getSocket());
    } catch (const QString& e) {
        qDebug() << e;
        delete thread;
        delete cv;
        return;
    }

    thread->start();
    cv->moveToThread(thread);

    QObject::connect(this, SIGNAL(signalStopVoiceComponent()),cv, SLOT(slotStopVoiceComponent()));
    connect(appClient_, SIGNAL(signalVoiceMessage(char*, int)),cv,SLOT(receiveData(char*,int)));
    cv->start();

}

void MainWindow::stopVoice() {
    Packet pckt;
    pckt.data = 0;
    pckt.length = 0;
    pckt.type = kVoice;
    emit signalStopVoiceComponent();
    appClient_->getSocket()->transmit(pckt);

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

void MainWindow::playSong() {
    if(ui->playButton->text() == "Tune In") {
        ui->playButton->setText("Play");
        stream_->start();
    } else {
        notes_.start();
        player_->play();
    }
}

void MainWindow::pauseSong() {
    notes_.stop();
    player_->pause();
}

void MainWindow::rate(int num) {
    static int sum = 0;
    sum += num;

    ui->rate->setText(QString::number(sum));
}
