#ifndef SERVERFILETRANSFER_H
#define SERVERFILETRANSFER_H

#include <QObject>
#include <QFile>
#include <QString>
#include "socket.h"
#include "Thread.h"

#define FT_PACKET_SIZE 4096
#define FT_HEADER_SIZE (sizeof(int) + sizeof(ComponentType))
#define FT_DATA_SIZE   (FT_PACKET_SIZE - FT_HEADER_SIZE)

class serverFileTransfer : public QObject
{
    Q_OBJECT
private:
    QString fileName_;
    Socket *socket_;
public:
    serverFileTransfer(QString, Socket *);
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

#endif // SERVERFILETRANSFER_H
