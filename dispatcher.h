#ifndef DISPATCHER_H
#define DISPATCHER_H

#include "components.h"

/**
 * The Dispatcher is the core to this program.
 *
 * All threads in this program are controlled through the dispatcher.
 * The dispatcher is run is run in its own thread although the
 * start/stop components will likly be called from the gui thread.
 *
 * All transmissions on the socket should be transmitted from
 * dispatcher.
 *
 * How the clients will handle multiple connections tho i have no idea
 * here
 * TODO
 *
 * @author Warren Voelkl
 */
class Dispatcher {

public:
    void dispatch(char* data, int length);

private:
    Components components_;

};

#endif // DISPATCHER_H
