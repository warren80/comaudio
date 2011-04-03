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
    connect(ui->refreshServerFilesButton, SIGNAL(pressed()), this, SLOT(refreshFiles()));

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

    if (connected) {
        Thread *thread = new Thread();
        streamServer_ = new ServerStream();
        connect(this, SIGNAL(playThisSong(QString)), streamServer_, SLOT(slotStartTransfer(QString)));
        connect(streamServer_, SIGNAL(signalTransferDone()), thread, SLOT(deleteLater()));
        connect(this, SIGNAL(stopThisSong()), streamServer_, SLOT(slotCleanup()));
        streamServer_->moveToThread(thread);
        thread->start();
    }

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

    appClient_->start();
    stream_->start();

    cylon_.start();
    clientConnect(true);
}

void MainWindow::appDisconnectClient() {
    delete appClient_;
    delete stream_;
    ui->playButton->setText("Tune In");
    ui->serverFilesView->clear();
    clientConnect(false);

}

void MainWindow::appStartServer() {
    setWindowTitle("Kidnapster - Server");
    try {
        appServer_ = new Server(htons(8001));
    } catch (const QString& e) {
        qDebug() << e;
    }
    connect(this, SIGNAL(stopThisSong()), appServer_, SLOT(slotDisconnectStream()));

    appServer_->start();
    refreshSongList();
    serverConnect(true);
}

void MainWindow::appStopServer() {
    delete appServer_;
    ui->songList->clear();
    serverConnect(false);
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

/*
* TODO:
* 1. Send request to update files
* 2. Update list widget
*/
void MainWindow::refreshFiles() {
    QDir dir("C:\\Users\\KCastillo\\Documents");
    QString filter = ui->filterBox->text();
    if(filter.length() == 0) {
        filter = "*.*";
    } else {
        filter = "*." + filter;
    }
    QStringList files = dir.entryList(QStringList(filter));

    ui->serverFilesView->clear();
    ui->serverFilesView->addItems(files);
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

}

void MainWindow::refreshSongList() {
    QDir dir("./Songs");
    QStringList files = dir.entryList(QStringList("*.wav"));

    ui->songList->clear();
    ui->songList->addItems(files);
}

void MainWindow::startVoice() {
    Packet pckt;
    pckt.data = 0;
    pckt.length = 0;
    pckt.type = kVoice;
    Thread *thread = new Thread();
    thread->start();
    appClient_->getSocket()->transmit(pckt);
    ComponentVoice *cv = new ComponentVoice(appClient_->getSocket());
    cv->moveToThread(thread);

    QObject::connect(this, SIGNAL(signalStopVoiceComponent()),cv, SLOT(slotStopVoiceComponent()));

}

void MainWindow::stopVoice() {
    Packet pckt;
    pckt.data = 0;
    pckt.length = 0;
    pckt.type = kVoice;
    emit signalStopVoiceComponent();
    appClient_->getSocket()->transmit(pckt);

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
