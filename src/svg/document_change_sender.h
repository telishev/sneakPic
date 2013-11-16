#ifndef DOCUMENT_CHANGE_SENDER_H
#define DOCUMENT_CHANGE_SENDER_H

#include <QObject>

class events_queue;
class items_edit_handler_t;
class QTimer;

class document_change_sender : public QObject
{
  Q_OBJECT

  events_queue *m_queue;
  items_edit_handler_t *m_items_edit_handler;
  QTimer *m_timer;
  int m_cur_event_id;
  bool m_pending_redraw;

public:
  document_change_sender (events_queue *queue, items_edit_handler_t *items_edit_handler);
  ~document_change_sender ();

  void update ();
  void set_redraw_needed ();

private slots:
  void check_for_redraw ();
  void send_changes (bool need_to_clear);
};

#endif // DOCUMENT_CHANGE_SENDER_H
