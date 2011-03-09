#ifndef COMPONENT_H
#define COMPONENT_H

#include "includes.h"

class Component : public QObject
{
    Q_OBJECT
public:
    Component();

protected:
    int port_;
signals:
    emit void signalTxPckt(void *);

public slots:
    virtual void slotPacketReceived(void *);
    virtual void slotStart() = 0;
};

#endif // COMPONENT_H
