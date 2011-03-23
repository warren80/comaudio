#ifndef COMPONENTiTERATOR_H
#define COMPONENTiTERATOR_H

#include <QObject>
#include <QMap>
#include "component.h"

#define COMPONENTMAX 251

//TODO fill in all the classes
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
     * Used prior to creating a component and adding it to the list
     * @returns a -1 if more COMPONENTMAX are already inexistence
     *          or 0 - 251 for the new component ID
     */
    int reserveID();

signals:

public slots:
};

#endif // ComponentIterator_H
