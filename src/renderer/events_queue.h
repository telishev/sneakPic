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

  bool m_interrupt;
public:
  events_queue ();
  ~events_queue ();

  int add_event (abstract_renderer_event *ev);
  int add_event_and_wait (abstract_renderer_event *ev, unsigned long ms = 50);

  abstract_renderer_event *pull_event ();
  void timed_wait (unsigned long time);
  void wait_for_id (int id, unsigned long time_ms);
  void set_calculated_id (int id);

  void lock ();
  void unlock ();

  int empty () const;
  int empty_nolock () const;

  bool is_interrupted () const;
  void reset_interrupt ();

  bool is_event_finished (int event_id);
};

#endif // EVENTS_QUEUE_H
