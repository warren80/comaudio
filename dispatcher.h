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



class Dispatcher : public QObject
{
    Q_OBJECT
public:
    explicit Dispatcher(QObject *parent = 0);
    void startComponent(int type);
    void stopComponent(int type);
protected:
private:
    Socket * pSocket;
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
