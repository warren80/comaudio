#include "componentstream.h"

ComponentStream::ComponentStream(Socket socket) : Component(kStream, socket) {

}

void ComponentStream::receiveData(char *data, int length) {
    player_.appendBuffer(data, length);
}
