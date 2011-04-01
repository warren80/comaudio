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
    /**
     * Constructs an object which will push the data of a song
     * onto a multicast port
     * @author Warren Voelkl
     */

    explicit ServerStream(QString fileNamePath);
public slots:

    /**
     * The worker part of this worker thread It sets up a multicast port
     * pushes the data over the port then removes the port.
     * @author Warren Voelkl
     */
    void slotStartTransfer();
signals:
    void signalTransferDone();
};

#endif // SERVERSTREAM_H
