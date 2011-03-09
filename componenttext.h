#ifndef COMPONENTTEXT_H
#define COMPONENTTEXT_H

#include "includes.h"
#include "component.h"

class ComponentText : public Component
{
    Q_OBJECT
public:
    ComponentText();
public slots:
    virtual void slotStart();
};

#endif // COMPONENTTEXT_H
