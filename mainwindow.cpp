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
    ComponentVoice *cv;
    QValidator *validPort = new QRegExpValidator(QRegExp("^\\d*$"), this);
    QValidator *validIp = new QRegExpValidator(QRegExp("^\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}$"), this);

    ui->setupUi(this);

    //Setting regex validation
    ui->serverAddrBox->setValidator(validIp);
    ui->portBox->setValidator(validPort);

    /**
     * CONNECTIONS
     */
    //Chat
    connect(ui->sendButton, SIGNAL(pressed()), this, SLOT(sendText()));

    //Settings Tab
    connect(ui->connectButton, SIGNAL(pressed()), this, SLOT(appConnect()));
    connect(ui->disconnectButton, SIGNAL(pressed()), this, SLOT(appDisconnect()));

    //Files Tab
    connect(ui->browseButton, SIGNAL(pressed()), this, SLOT(browseFile()));
    connect(ui->sendFileButton, SIGNAL(pressed()), this, SLOT(sendFile()));
    connect(ui->downloadSongButton, SIGNAL(pressed()), this, SLOT(downloadSong()));
    connect(ui->downloadCurrentSongButton, SIGNAL(pressed()), this, SLOT(downloadCurrentSong()));
    connect(ui->refreshServerFilesButton, SIGNAL(pressed()), this, SLOT(refreshFiles()));

    //Audio Player
    connect(ui->previousButton, SIGNAL(pressed()), this, SLOT(previousSong()));
    connect(ui->playButton, SIGNAL(pressed()), this, SLOT(playSong()));
    connect(ui->pauseButton, SIGNAL(pressed()), this, SLOT(pauseSong()));
    connect(ui->nextButton, SIGNAL(pressed()), this, SLOT(nextSong()));

    //testing for the voice component
    cv = new ComponentVoice();
    cv->start();
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
    ui->typeScreen->setEnabled(settings_->isClient && connected);
    ui->sendButton->setEnabled(settings_->isClient && connected);
    ui->fileTab->setEnabled(settings_->isClient && connected);
    ui->previousButton->setEnabled(settings_->isClient && connected);
    ui->playButton->setEnabled(settings_->isClient && connected);
    ui->pauseButton->setEnabled(settings_->isClient && connected);
    ui->nextButton->setEnabled(settings_->isClient && connected);

    if(!connected) {
        if(settings_->isClient) {
            //mic_->stopRecording();
            //mic_->deleteLater();
            //delete player_;
        }
        delete settings_;
        ui->statusText->setText("Disconnected");
    }
}

void MainWindow::printF(const char *message) {
    ui->chatScreen->appendPlainText(QString(message));
}

void MainWindow::printF(const QString message) {
    ui->chatScreen->appendPlainText(message);
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

    settings_->port = ui->portBox->text().toInt();

    if((settings_->isClient = ui->client->isChecked())) {

        //Settings
        ui->statusText->setText("Client");
        setWindowTitle("Kidnapster - Client");
        settings_->ipAddr = ui->serverAddrBox->text();
        settings_->alias = ui->aliasBox->text();
        settings_->logChat = ui->logChatBox->isChecked();

        appClient_ = new Client();
        if (!appClient_->connect(inet_addr(settings_->ipAddr.toStdString().c_str()), htons(settings_->port))) {
            delete appClient_;
            return;
        }
        appClient_->start();

    } else {
        ui->statusText->setText("Server");
        setWindowTitle("Kidnapster - Server");
        appServer_ = new Server(htons(settings_->port));
        appServer_->start();
    }

    connected(true);
}

//TODO: Close socket and delete socket/client/server objects
void MainWindow::appDisconnect() {
    qDebug("Disconnecting");
    if(settings_->logChat) {
        chatLog_ = new Logs(QString("./logs/chat_log_" +
                                    QString::number(QDateTime::currentMSecsSinceEpoch()) + ".log"),
                            QDateTime::currentDateTime().toString());
        chatLog_->writeToLog(ui->chatScreen->toPlainText());
    }
    ui->chatScreen->clear();
    ui->serverFilesView->clear();
    setWindowTitle("Kidnapster - Disconnected");
    connected(false);
}

/**
 * CHAT
 */

//TODO: Send text to the server
void MainWindow::sendText() {
    QString message = ui->typeScreen->toPlainText();

    ui->typeScreen->clear();
    printF(settings_->alias + ":\n" + message);
}

void MainWindow::browseFile() {
    QString fileNamePath = QFileDialog::getOpenFileName(this, "Open", QDir::homePath(), "Text (*.txt);;All(*.*)");
    ui->pathBox->setText(fileNamePath);

}

//TODO: Send file to the server
void MainWindow::sendFile() {
    QString fileNamePath = ui->pathBox->text();

    if(fileNamePath.length() != 0) {

    }
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
void MainWindow::previousSong() {

}

void MainWindow::playSong() {
    player_->play();
}

void MainWindow::pauseSong() {
    player_->pause();
}

void MainWindow::nextSong() {

}

/**
 * VOICE
 */
void MainWindow::sendVoice(const char *mesg) {
    //printF(mesg);
    //transmit!!!
}

