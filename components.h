#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <QVector>
#include "component.h"

class Components
{
public:
    Components();

    /**
      Get the component at the requested ID.

      @param id ID of component to retrieve.
      @return The requested componented.
      @author Nick Huber
      */
    Component getComponent(int id) { return *components_[id]; };

private:
    QVector<Component*> components_;
};

#endif // COMPONENTS_H
