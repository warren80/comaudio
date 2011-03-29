#ifndef COMPONENTSTREAM_H
#define COMPONENTSTREAM_H

#include "component.h"
#include "audioplayer.h"
class ComponentStream : public Component {
public:
    ComponentStream(Socket socket);

    void newData(char *data, int length);
private:
    AudioPlayer player_;
};

#endif // COMPONENTSTREAM_H
