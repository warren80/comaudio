#ifndef DISPATCHER_H
#define DISPATCHER_H

#include "includes.h"
#include "socket.h"
#include "component.h"
#include "componentaudio.h"
#include "componentfile.h"
#include "componenttext.h"
#include "componentvoice.h"
#include "ComponentIterator.h"


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
public:
    Dispatcher(QObject *parent = 0);
    void startComponent(int type, int socketID);
    void stopComponent(int type);

protected:
private:
    ComponentIterator compIterator_;
signals:
    void signalTxPckt(Message * msg);
public slots:
    void slotPacketRecieved(Packet * msg);
    void slotPacketToTransmit(Message * msg);
    void slotSocketClosed(int socketID);

};

#endif // DISPATCHER_H
