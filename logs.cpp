#include "logs.h"
#include <QDateTime>

Logs::Logs(QString pathName, QString title):
        log_(pathName)
{   
    if(!log_.open(QIODevice::WriteOnly)) {
        qDebug("Cannot open log");
    }
    log_.write(title.toLatin1());
    log_.close();
}

void Logs::writeToLog(const char *error, int errNo) {
    QString errorMesg(error);
    errorMesg +=  " " + QDateTime::currentDateTime().toString();
    log_.open(QIODevice::Append);
    log_.write(errorMesg.toLatin1().data());
    log_.write("Errno: " + errNo);
    log_.close();
}

void Logs::writeToLog(const char *message) {
    log_.open(QIODevice::Append);
    log_.write(message);
    log_.close();
}

void Logs::writeToLog(QString message) {
    log_.open(QIODevice::Append);
    log_.write(message.toLatin1());
    log_.close();
}
