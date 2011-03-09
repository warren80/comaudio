#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->sendButton, SIGNAL(clicked()), this, SLOT(sendMessage()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::printF(const char *message) {
    ui->chatScreen->appendPlainText(QString(*message));
}

void MainWindow::printF(QString message) {
    ui->chatScreen->appendPlainText(message);
}

/*
void MainWindow::on_actionConnect_triggered() {
    Settings *settingsDiag = new Settings(this);

    settingsDiag->exec();

    settings = settingsDiag->getSettings();

    if(settingsDiag->result()) {
        //TODO: Connect to server/Listen for clients
        if(settings->isClient) {
            //move these to be started when button pressed in gui
            qDebug("Client");
            setWindowTitle("Los Ostrich - Client");
            enableChat(true);

            qDebug(settings->ipAddr.toLatin1().data());
            qDebug(QString::number(settings->port).toLatin1().data());
            qDebug(settings->alias.toLatin1().data());

            //TODO get a port from gui and ip just a hack here to make it compile
            char *ip = settings->ipAddr.toLatin1().data();
            //TextClient * tc = new TextClient(ip, settings->port, BUFSIZE);
            textClient = new Thread();
            textClient->start();
            //connect(tc,SIGNAL(signalTextRecieved(TextReceived*)),
            //        this,SLOT(slotTextRecieved(TextReceived*)));
            tc->moveToThread(textClient);
            connect(this, SIGNAL(startSignalClient()), tc, SLOT(Start()));
            emit startSignalClient();
        } else {

        }
    } else {
        qDebug("Settings Cancelled");
    }
}


*/
