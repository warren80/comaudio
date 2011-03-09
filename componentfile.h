#ifndef COMPONENTFILE_H
#define COMPONENTFILE_H

#include "includes.h"
#include "component.h"

class ComponentFile : public Component
{
    Q_OBJECT
public:
    ComponentFile();
public slots:
    virtual void slotStart();
};

#endif // COMPONENTFILE_H
