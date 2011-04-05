#ifndef SERVERFILETRANSFER_H
#define SERVERFILETRANSFER_H

#include <QObject>
#include <QFile>
#include <QString>
#include <QMutex>
#include "socket.h"
#include "thread.h"

#define FT_PACKET_SIZE 4096
#define FT_HEADER_SIZE (sizeof(int) + sizeof(ComponentType))
#define FT_DATA_SIZE   (FT_PACKET_SIZE - FT_HEADER_SIZE)

class ServerFileTransfer : public QObject
{
    Q_OBJECT
private:
    QString fileName_;
    Socket *socket_;
    QMutex fileTransferInProgress;
    void cleanup();
public:
    /**
     * Constructor for a file transfer object.
     *
     * This is a one shot object that transmits a
     * file and then ends upon compleation.
     * @author Warren Voelkl
     */
    ServerFileTransfer(QString, Socket *);
    ~ServerFileTransfer();
public slots:
    /**
     * The worker part of this worker thread It sets up a multicast port
     * pushes the data over the port then removes the port.
     * @author Warren Voelkl
     */
    void slotStartTransfer();
signals:
    /**
      File transfer completed.

      @author Nick Huber
      */
    void signalTransferDone();
};
#endif // SERVERFILETRANSFER_H
