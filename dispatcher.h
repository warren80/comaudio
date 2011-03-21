#ifndef DISPATCHER_H
#define DISPATCHER_H

#include "includes.h"
#include "socket.h"
#include "component.h"
#include "componentaudio.h"
#include "componentfile.h"
#include "componenttext.h"
#include "componentvoice.h"
#include "thread.h"


/**
 * The Dispatcher is the core to this program.
 *
 * All threads in this program are controlled through the dispatcher.
 * The dispatcher is run is run in its own thread although the
 * start/stop components will likly be called from the gui thread.
 *
 * All transmissions on the socket should be transmitted from
 * this dispatcher thread.  All recieves will be run in its own
 * recieve thread.
 *
 * How the clients will handle multiple connections tho i have no idea
 *here
 * TODO
 * buffering transmissions maybe?  This may be done implicitly on our signal slot system.
 * may want to rework the design to do our own buffering inside the Socket class
 * and have trasmissions in their own thread not this one.
 *
 * @author Warren Voelkl
 */

class Dispatcher : public QObject
{
    Q_OBJECT
public:
    explicit Dispatcher(QObject *parent = 0);
    void startComponent(int type);
    void stopComponent(int type);
protected:
private:
    SocketClass * pSocket;
signals:
    void signalStartComponentAudio();
    void signalStopComponentAudio();
    void signalStartComponentFile();
    void signalStopComponentFile();
    void signalStartComponentText();
    void signalStopComponentText();
    void signalStartComponentVoice();
    void signalStopComponentVoice();

    void signalPacketToComponentAudio(void *);
    void signalPacketToComponentFile(void *);
    void signalPacketToComponentText(void *);
    void signalPacketToComponentVoice(void *);

public slots:
    void slotPacketRecieved(void *);
    void slotPacketToTransmit(void*);
};

#endif // DISPATCHER_H
