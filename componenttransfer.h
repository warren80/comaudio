#ifndef COMPONENTTRANSFER_H
#define COMPONENTTRANSFER_H


#include <QString>
#include "component.h"

class ComponentTransfer : public Component {
public:
    Q_OBJECT
    ComponentTransfer(Socket* socket, QString fileName);
    void run();
private:
    QString fileName_;

};

#endif // COMPONENTTRANSFER_H
