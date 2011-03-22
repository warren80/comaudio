#ifndef COMPONENTFILE_H
#define COMPONENTFILE_H

#include "includes.h"
#include "component.h"

class ComponentFile : public Component
{
    Q_OBJECT
public:
    ComponentFile();

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

public slots:
    virtual void slotStart();
};

#endif // COMPONENTFILE_H
