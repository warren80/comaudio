#include "componentstream.h"

ComponentStream::ComponentStream(Socket socket) : Component(kStream, socket) {

}

void ComponentStream::newData(char *data, int length) {
    player_.appendBuffer(data, length);
}

