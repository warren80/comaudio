#ifndef COMPONENTITERATOR_H
#define COMPONENTITERATOR_H

#include <QObject>
#include <QMap>
#include "component.h"

class componentIterator : public QObject
{
    Q_OBJECT
public:
    explicit componentIterator(QObject *parent = 0);
    QMap<Component, unsigned char> componentMap_;

    addComponent(Component comp);
    removeComponent(unsigned char);
signals:

public slots:

};

#endif // COMPONENTITERATOR_H
