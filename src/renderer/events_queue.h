#ifndef EVENTS_QUEUE_H
#define EVENTS_QUEUE_H

class abstract_renderer_event;
class QMutex;
class QWaitCondition;

#include <QQueue>

class events_queue
{
  int m_last_queue_id;
  int m_calculated_id;

  QMutex *m_mutex;
  QWaitCondition *m_wait_cond;
  QQueue<abstract_renderer_event *> m_queue;
public:
  events_queue ();
  ~events_queue ();

  int add_event (abstract_renderer_event *ev);

  abstract_renderer_event *pull_event ();
  void timed_wait (unsigned long time);
  void wait_for_id (int id, unsigned long time_ms);
  void set_calculated_id (int id);

  void lock ();
  void unlock ();

  int empty () const;
};

#endif // EVENTS_QUEUE_H
