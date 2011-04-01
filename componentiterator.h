#ifndef COMPONENTITERATOR_H
#define COMPONENTITERATOR_H

#include <QObject>
#include <QMap>
#include "packet.h"
#include "component.h"
#include "componentaudio.h"
#include "componentfile.h"
#include "componentvoice.h"

//TODO fill in all the functions
//Decide on how to loop through list either in the creator or from a slot
//slot could be signled after a compleat list through this file or
//a continuous loop.
//Warren

class ComponentIterator : public QObject
{
    Q_OBJECT
public:
    explicit ComponentIterator(QObject *parent = 0);
    QMap<Component, unsigned char> componentMap_;

    /**
     * Used to add a component to the iterator list
     */
    void addComponent(Component comp);
    /**
     * Used to remove a component from the iterator list
     * will also call the destructor of the component
     */
    void removeComponent(unsigned char);
    /**
     * Used to create a component prior to adding it to the componentlist
     * @returns a -1 if more COMPONENENT limit hit
     *          or 0 - 251 for the new component ID
     */
    int createComponent(Message * msg);
    /**
     * gives client packets recieved from socket to the
     * component.
     *
     * @returns -1 if component does not exist;
     */
    int clientMessage(Message * msg);

signals:

public slots:
};

#endif // ComponentIterator_H
