#ifndef DISPATCHER_H
#define DISPATCHER_H

#include "includes.h"
#include "socket.h"
#include "componentiterator.h"

/**
 * The Dispatcher is the core to this program.
 *
 * All threads in this program are controlled through the dispatcher.
 * The dispatcher is run is run in its own thread although the
 * start/stop components will likly be called from the gui thread.
 *
 * All transmissions on the socket should be transmitted from
 * dispatcher.
 *
 * How the clients will handle multiple connections tho i have no idea
 * here
 * TODO
 *
 * @author Warren Voelkl
 */

class Dispatcher : public QObject
{
Q_OBJECT
private:
    /** the class which holds all components */
    ComponentIterator * compIterator_;

public:
    /**
     * Constructor creates the compIterator
     *
     * @author Warren Voelkl
     */
    Dispatcher(QObject *parent = 0);
    /**
     * tells the componentIterator class to create a new component
     *
     * @author Warren Voelkl
     */
    void startComponent(Message * msg);
    /**
     *Tells the componentIterator class to delete a component
     *
     *@author Warren Voelkl
     */
    void stopComponent(int type);

protected:

signals:
    void signalTxPckt(Message * msg);
public slots:
    void slotPacketRecieved(Message * msg);
    void slotPacketToTransmit(Message * msg);
    void slotSocketClosed(int socketID);

};

#endif // DISPATCHER_H
