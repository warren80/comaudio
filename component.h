#ifndef COMPONENT_H
#define COMPONENT_H

#include "socket.h"

enum ComponentType {
    kChat,      /**< Textual chat. */
    kStream,    /**< Music streaming. */
    kTransfer,  /**< File transfer. */
    kVoice,     /**< Multicast voice chat. */
};

class Component {

public:
    Component(ComponentType type, Socket socket);

private:
    ComponentType type_;
    Socket socket_;

};

#endif // COMPONENT_H
