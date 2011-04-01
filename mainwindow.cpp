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

/**
 * CONSTRUCTOR
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QValidator *validPort = new QRegExpValidator(QRegExp("^\\d*$"), this);
    QValidator *validIp = new QRegExpValidator(QRegExp("^\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}$"), this);

    ui->setupUi(this);

    //Setting regex validation
    ui->serverAddrBox->setValidator(validIp);
    ui->portBox->setValidator(validPort);

    /**
     * CONNECTIONS
     */

    //Settings Tab
    connect(ui->connectButton, SIGNAL(pressed()), this, SLOT(appConnect()));
    connect(ui->disconnectButton, SIGNAL(pressed()), this, SLOT(appDisconnect()));

    //Files Tab
    connect(ui->downloadSongButton, SIGNAL(pressed()), this, SLOT(downloadSong()));
    connect(ui->downloadCurrentSongButton, SIGNAL(pressed()), this, SLOT(downloadCurrentSong()));
    connect(ui->refreshServerFilesButton, SIGNAL(pressed()), this, SLOT(refreshFiles()));

    //Audio Player
    connect(ui->playButton, SIGNAL(pressed()), this, SLOT(playSong()));
    connect(ui->pauseButton, SIGNAL(pressed()), this, SLOT(pauseSong()));

    notes_.setFileName(".\\notes.gif");
    cylon_.setFileName(".\\cylon.gif");
    ui->cylon->setMovie(&cylon_);
    ui->notes->setMovie(&notes_);
}

/**
 * DESTRUCTOR
 */
MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::initDispatcher() {
    Thread * dispatcherThread = new Thread();
    dispatcherThread->start();

}

/**
 * HELPERS
 */
void MainWindow::connected(bool connected) {
    ui->connectButton->setEnabled(!connected);
    ui->disconnectButton->setEnabled(connected);

    //If client
    ui->fileTab->setEnabled(settings_->isClient && connected);
    ui->playButton->setEnabled(settings_->isClient && connected);
    ui->pauseButton->setEnabled(settings_->isClient && connected);

    //If Server
    ui->serverTab->setEnabled(!settings_->isClient && connected);

    if(!connected) {
        cylon_.stop();
        notes_.stop();
        if(settings_->isClient) {
            //delete player_;
        }
        delete settings_;
        ui->statusText->setText("Disconnected");
    } else {
        cylon_.start();
    }
}

/**
 * SLOTS
 */

/**
 * SETTINGS TAB
 */

//TODO: Connect
void MainWindow::appConnect() {
    settings_ = new Settings();
    qDebug("Connecting...");

    if((settings_->isClient = ui->client->isChecked())) {
        player_ = new AudioPlayer();

        //Settings
        ui->statusText->setText("Client");
        setWindowTitle("Kidnapster - Client");
        settings_->ipAddr = ui->serverAddrBox->text();
        settings_->alias = ui->aliasBox->text();
        settings_->logChat = ui->logChatBox->isChecked();

        appClient_ = new Client(settings_->ipAddr.toLatin1().data()
                                , settings_->port, settings_->alias);
        appClient_->start();
    } else {
        ui->statusText->setText("Server");
        setWindowTitle("Kidnapster - Server");
        appServer_ = new Server(htons(settings_->port));
        appServer_->start();
    }

    settings_->port = ui->portBox->text().toInt();
    qDebug() << QString::number(settings_->port).toLatin1().data();

    cylon_.start();
    connected(true);
}

//TODO: Close socket and delete socket/client/server objects
void MainWindow::appDisconnect() {
    qDebug("Disconnecting");

    ui->serverFilesView->clear();
    setWindowTitle("Kidnapster - Disconnected");
    connected(false);
}

/**
 * FILE TAB
 */

/*
 * TODO:
 *  1. Send request
 */
void MainWindow::downloadSong() {
    QString songName = ui->serverFilesView->currentItem()->text();

}

/*
 * TODO:
 *  1. Send Request
 */
void MainWindow::downloadCurrentSong() {
    QString songName = ui->currentSongText->text();

}

/*
 * TODO:
 *  1. Send request to update files
 *  2. Update list widget
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

/**
 * AUDIO PLAYER
 */

void MainWindow::playSong() {
    notes_.start();
    player_->play();
}

void MainWindow::pauseSong() {
    notes_.stop();
    player_->pause();
}

