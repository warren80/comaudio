#ifndef SERVERSTREAM_H
#define SERVERSTREAM_H
#include <QObject>
#include <QFile>
#include <QTimer>
#include "Socket.h"
#include "Thread.h"

#define BYTERATEOFFSET 28
#define BYTERATESIZE    4

#include "componentstream.h"

class ServerStream :  public QObject {
Q_OBJECT
private:
    bool    cleanup_;   /**< a variable set when the obect cleanup is started */
    QString fileName_;  /**< The Filename of the file to be transmitted */
    QTimer  *timer_;    /**< The timer object that will signal the transmit fn */
    QFile   *file_;     /**< The opened file descriptor to be read from */
    Socket  *socket_;   /**< The Socket which the data will be transmitted on */
    char    *buffer_;   /**< The container for the data to be transmitted */
public:
    /**
     * Constructs an object which will push the data of a song
     * onto a multicast port
     * @author Warren Voelkl
     */
    explicit ServerStream();
public slots:

    /**
     * Sets up the initial information for transmitting a file
     * and signals when the obect should be started also
     * sets up the timer for this object
     * @author Warren Voelkl
     */
    void slotStartTransfer(QString filename);
    /**
     * When the timer ticks this function will packetize some data
     * and transmit it.
     * @author Warren Voelkl
     */
    void slotTransmitOnTimer();
    void slotCleanup();
signals:
    void signalCleanup();
    void signalTransferDone();

};

#endif // SERVERSTREAM_H
