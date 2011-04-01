#ifndef COMPONENT_H
#define COMPONENT_H

#include "includes.h"

class Component : public QObject
{
    Q_OBJECT
public:
    Component(int socketID);

protected:
    int port_;
    int socketID_;

signals:
    emit void signalTxPckt(void *);

public slots:
    virtual void slotPacketReceived(void *);
    virtual void slotStart();
};

#endif // COMPONENT_H
