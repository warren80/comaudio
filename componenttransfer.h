#ifndef COMPONENTTRANSFER_H
#define COMPONENTTRANSFER_H


#include <QString>
#include "component.h"

class ComponentTransfer : public Component {
public:
    ComponentTransfer(const Socket& socket);

private:
    /**
     * Call this function to open the file that will be sent and packetize it.
     *
     * @author Karl Castillo
     * @arg fileNamePath - the path to the file that the user wants to send
     */
    void setupFileTransfer(QString fileNamePath);
    /**
     * Call this function to save whatever is received by the server.
     *
     * @author Karl Castillo
     * @arg mesg - the received message
     */
    void saveFile(QList<char*> mesg);
};

#endif // COMPONENTTRANSFER_H
