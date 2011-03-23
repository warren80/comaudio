#ifndef COMPONENTAUDIO_H
#define COMPONENTAUDIO_H

#include "includes.h"
#include "component.h"

class ComponentAudio : public Component
{
    Q_OBJECT
public:
    ComponentAudio(int socketID);
public slots:
    virtual void slotStart();
};

#endif // COMPONENTAUDIO_H
