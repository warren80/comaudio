#ifndef THREAD_H
#define THREAD_H

#include <QThread>

/**
  Thread to give each thread its own event loop.

  @author Warren Voelkl.
  */
class Thread : public QThread
{
protected:
    /**
      run() which simply calls exec() for an event loop.

      @author Warren Voelkl
      */
    virtual void run();
};

#endif
