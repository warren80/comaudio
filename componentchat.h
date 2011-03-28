#ifndef COMPONENTCHAT_H
#define COMPONENTCHAT_H

#include "component.h"

class ComponentChat : public Component {
public:
    ComponentChat(int socketID);
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
