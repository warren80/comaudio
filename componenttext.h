#ifndef COMPONENTTEXT_H
#define COMPONENTTEXT_H

#include "includes.h"
#include "component.h"

class ComponentText : public Component
{
    Q_OBJECT
public:
    ComponentText();
    /**
     * Call this function to send the message the user typed into the
     * chat input box.
     *
     * @author Karl Castillo
     * @arg message - the message that the user typed into the chat
     *                input box
     */
    void sendMessage(QString message);

public slots:
    virtual void slotStart();
};

#endif // COMPONENTTEXT_H
