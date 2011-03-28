#ifndef COMPONENTVOICE_H
#define COMPONENTVOICE_H

#include "component.h"

class ComponentVoice : public Component {

public:
    ComponentVoice(int socketID);
public slots:
    virtual void slotStart();
};

#endif // COMPONENTVOICE_H
