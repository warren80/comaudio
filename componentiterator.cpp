#include "componentiterator.h"

ComponentIterator::ComponentIterator(QObject *parent) :
    QObject(parent)
{
}

void ComponentIterator::addComponent(Component comp) {}

void ComponentIterator::removeComponent(unsigned char) {}

int ComponentIterator::createComponent(Message * msg) {
    Component * pComponent;
    switch (msg->payload->componentID) {
    case AUDIOCOMPONENT:
        pComponent = new ComponentAudio(msg->socketID);
        break;
    case FILECOMPONENT:
        pComponent = new ComponentFile(msg->socketID);
        break;
    case VOICECOMPONENT:
        pComponent = new ComponentVoice(msg->socketID);
        break;
    }
    return 0;
}

int ComponentIterator::clientMessage(Message * msg) { return -1;}
