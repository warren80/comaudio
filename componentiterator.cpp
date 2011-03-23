#include "componentiterator.h"

ComponentIterator::ComponentIterator(QObject *parent) :
    QObject(parent)
{
}

void ComponentIterator::addComponent(Component comp) {}

void ComponentIterator::removeComponent(unsigned char) {}

int ComponentIterator::reserveID() {return 0;}

int ComponentIterator::clientMessage(Message * msg) { return -1;}
