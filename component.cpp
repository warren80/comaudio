#include "component.h"
#include <QDebug>

Component::Component(Socket* socket) : socket_(socket) {

}

Component::~Component() {
}
