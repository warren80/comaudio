#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <QVector>
#include <QMutex>
#include <QSemaphore>
#include <QThread>
#include "component.h"

class Components : public QThread
{
public:
    Components();
    ~Components();
    /**
      Access operator for the components.

      @param index Component to get.
      @return Pointer to the component.
      @author Nick Huber
      */
    Component* operator[](int index) { return components_[index]; };

    /**
      Add a component and the associated pending state.

      @param component New component to add to the components collection.
      @author Nick Huber
      */
    int addComponent(Component* component);

    void addData(int index, char* data, int length);

protected:
    void run();

private:
    QVector<Component*> components_; /**< Every component. */
    QVector<bool> pending_;          /**< Goes in tandem with components_ to know if it has a pending operation. */
    QMutex componentsControl_;       /**< Sync. the components access. */
    QSemaphore waiting_;             /**< Control for unprocessed components.*/
    bool running_;                   /**< State of the thread. */
};

#endif // COMPONENTS_H
