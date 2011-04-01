#include "componenttransfer.h"
#include <QFile>
#include <QList>
#include <QMessageBox>

/**
 * CONSTRUCTOR
 */
ComponentTransfer::ComponentTransfer(const Socket& socket) : Component(socket) {
}

/**
 * ESSENTIAL METHODS
 */

/*
 * TODO:
 *  1. Save into buffer
 *  2. Send file
 */
void ComponentTransfer::setupFileTransfer(QString fileNamePath) {
    QFile *file = new QFile(fileNamePath);
    QList<char*> buffer;
    char *message;

    if(file->open(QIODevice::ReadOnly)) {
        QMessageBox::QMessageBox(QMessageBox::Critical, "Error", "Cannot open file or file doesn't exist", QMessageBox::Ok).exec();
        return;
    }

    while((message = file->read(1023).data()) > 0) {
        buffer.append(message);
    }

    file->close();
}

void ComponentTransfer::saveFile(QList<char*> mesg) {
    QFile *file = new QFile("test.txt");
    QListIterator<char*> iterator(mesg);

    if(file->open(QIODevice::ReadOnly)) {
        QMessageBox::QMessageBox(QMessageBox::Critical, "Error", "Cannot open file or file doesn't exist", QMessageBox::Ok).exec();
        return;
    }

    while(iterator.hasNext()) {
        file->write(iterator.peekNext(), 1024);
    }

    file->close();
}
