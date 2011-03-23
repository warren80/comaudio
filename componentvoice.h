#ifndef COMPONENTVOICE_H
#define COMPONENTVOICE_H

#include "includes.h"
#include "component.h"

class ComponentVoice : public Component
{
    Q_OBJECT
public:
    ComponentVoice(int socketID);
public slots:
    virtual void slotStart();
};

#endif // COMPONENTVOICE_H
