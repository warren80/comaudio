#ifndef COMPONENTSTREAM_H
#define COMPONENTSTREAM_H

#include "component.h"

class ComponentStream : public Component {
public:
    ComponentStream(Socket socket);
};

#endif // COMPONENTSTREAM_H
