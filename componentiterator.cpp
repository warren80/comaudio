#include "componentiterator.h"

componentIterator::componentIterator(QObject *parent) :
    QObject(parent)
{
}

void addComponent(Component comp) {}

void removeComponent(unsigned char) {}

int reserveID() {return 0;}
