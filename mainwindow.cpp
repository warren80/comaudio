#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initDispatcher();
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

void initDispatcher() {
    Thread * dispatcherThread = new Thread();
    dispatcherThread->start();

}
