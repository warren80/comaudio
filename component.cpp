#include "component.h"

Component::Component(Socket* socket) : socket_(socket) {

}

Component::~Component() {
    qDebug() <<  "Delete Component";
}
