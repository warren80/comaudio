#ifndef COMPONENT_H
#define COMPONENT_H

#include "includes.h"
#include "logs.h"

class Component : public QObject
{
    Q_OBJECT
public:
    Component();

protected:
    int port_;
    Logs *errorLog_;
    Logs *chatLog_;
    Logs *activityLog_;

signals:
    emit void signalTxPckt(void *);

public slots:
    virtual void slotPacketReceived(void *);
    virtual void slotStart() = 0;
};

#endif // COMPONENT_H
