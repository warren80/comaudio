#include "components.h"

Components::Components() {

}

Components::~Components() {
    running_ = false;
    // wait for the thread to end
    wait();
}

int Components::addComponent(Component *component) {
    componentsControl_.lock();
    components_ += component;
    pending_ += false;
    int size = components_.size();
    componentsControl_.unlock();
    return size;
}


void Components::addData(int index, char *data, int length) {
    componentsControl_.lock();
    components_[index]->newData(data, length);
    pending_[index] = true;
    waiting_.release();
    componentsControl_.unlock();
}

void Components::run() {

    running_ = true;
    while (running_) {
        waiting_.acquire();
        componentsControl_.lock();
        for (int i = 0; i < components_.size(); i++) {
            if (pending_[i]) {
                //process component_[i]
                break;
            }
        }
        componentsControl_.unlock();
    }
}
