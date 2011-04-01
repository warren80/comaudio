#ifndef SERVERSTREAM_H
#define SERVERSTREAM_H


#include <QObject>
#include "Thread.h"


#include "componentstream.h"

class ServerStream :  public QObject {
Q_OBJECT
private:
    QString fileNamePath_;
public:
    explicit ServerStream(QString fileNamePath);
public slots:
    void slotStartTransfer();
signals:
    void signalTransferDone();
};

#endif // SERVERSTREAM_H
