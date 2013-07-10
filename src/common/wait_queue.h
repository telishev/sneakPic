#ifndef WAIT_QUEUE_H
#define WAIT_QUEUE_H

#include <QMutex>
#include <QWaitCondition>
#include <QQueue>
#include <QMutexLocker>

#include "common/memory_deallocation.h"

template<typename T>
class wait_queue
{
public:

  ~wait_queue ()
  {
    T *data = 0;
    while ((data = queue.dequeue ()))
      {
        FREE (data);
      }
  }

  /// Push element to the tail of queue
  int push_back (T *data)
    {
      QMutexLocker ml (&lock_mutex);
      queue.enqueue (data);
      wait_cond.wakeOne ();
      return 0;
    }

  /// Push element to the tail of queue
  int push_back (const T &data)
    {
      return push_back (new T (data));
    }

  /// Pull element from the head of queue
  int pull_front (T **data)
    {
      QMutexLocker ml (&lock_mutex);
      return pull_front_nolock (data);
    }

  int pull_front_nolock (T **data)
    {
      if (!data)
        return -1;

      if (!queue.isEmpty ())
        *data = queue.dequeue ();
      else
        *data = 0;
      return 0;
    }

  void wait ()
  {
    timed_wait ((unsigned long)-1);
  }

  void timed_wait (unsigned long time)
  {
    wait_cond.wait (&lock_mutex, time);
  }

  void lock ()
  {
    lock_mutex.lock ();
  }

  void unlock ()
  {
    lock_mutex.unlock ();
  }

  int empty () const
  {
    QMutexLocker ml (&lock_mutex);
    return empty_nolock ();
  }

  int empty_nolock () const
  {
    return queue.isEmpty ();
  }

private:

  QMutex lock_mutex;
  QWaitCondition wait_cond;

  QQueue<T *> queue;
};

#endif // WAIT_QUEUE_H
