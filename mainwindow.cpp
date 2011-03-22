#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>

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

    //Connections
    connect(ui->connectButton, SIGNAL(pressed()), this, SLOT(appConnect()));
    connect(ui->disconnectButton, SIGNAL(pressed()), this, SLOT(appDisconnect()));
    connect(ui->browseButton, SIGNAL(pressed()), this, SLOT(browseFile()));
    connect(ui->sendFileButton, SIGNAL(pressed()), this, SLOT(sendFile()));
    connect(ui->sendButton, SIGNAL(pressed()), this, SLOT(sendText()));

    initDispatcher();
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
    ui->typeScreen->setEnabled(settings_->isClient && connected);
    ui->sendButton->setEnabled(settings_->isClient && connected);
    ui->fileTab->setEnabled(settings_->isClient && connected);

    if(connected) {
        if(settings_->isClient) {
            ui->statusText->setText("Client");
        } else {
            ui->statusText->setText("Server");
        }
    } else {
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
//TODO: Create socket and connect
void MainWindow::appConnect() {
    settings_ = new SETTINGS;

    if((settings_->isClient = ui->client->isChecked())) {
        setWindowTitle("Kidnapster - Client");
        settings_->ipAddr = ui->serverAddrBox->text();
        settings_->alias = ui->aliasBox->text();
        qDebug(settings_->ipAddr.toLatin1().data());
        qDebug(settings_->alias.toLatin1().data());
    } else {
        setWindowTitle("Kidnapster - Server");
    }

    settings_->port = ui->portBox->text().toInt();
    qDebug(QString::number(settings_->port).toLatin1().data());

    connected(true);
}

//TODO: Close socket and delete socket/client/server objects
void MainWindow::appDisconnect() {
    setWindowTitle("Kidnapster - Disconnected");
    connected(false);
}

void MainWindow::browseFile() {
    QString fileNamePath = QFileDialog::getOpenFileName(this, "Open", QDir::homePath(), "Text (*.txt);;All(*.*)");
    ui->pathBox->setText(fileNamePath);
}

//TODO: Send file to the server
void MainWindow::sendFile() {

}

//TODO: Send text to the server
void MainWindow::sendText() {
    QString message = ui->typeScreen->toPlainText();

}


