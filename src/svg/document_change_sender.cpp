#include "svg/document_change_sender.h"

#include <QTimer>

#include "common/memory_deallocation.h"

#include "gui/connection.h"

#include "renderer/events_queue.h"
#include "renderer/event_items_changed.h"

#include "svg/changed_items_container.h"

document_change_sender::document_change_sender (events_queue *queue, items_edit_handler_t *items_edit_handler)
{
  m_queue = queue;
  m_items_edit_handler = items_edit_handler;
  m_cur_event_id = -1;
  m_pending_redraw = false;
  m_timer = new QTimer (this);
  CONNECT (m_timer, &QTimer::timeout, this, &document_change_sender::check_for_redraw);
  m_timer->setInterval (20);
  m_timer->start ();
}

document_change_sender::~document_change_sender ()
{
  FREE (m_timer);
}

void document_change_sender::set_redraw_needed ()
{
  m_pending_redraw = true;
  check_for_redraw ();
}

void document_change_sender::update ()
{
  send_changes (true);
  m_queue->wait_for_id (m_cur_event_id, 50);
}

void document_change_sender::check_for_redraw ()
{
  if (!m_pending_redraw)
    return;

  if (m_cur_event_id >= 0 && !m_queue->is_event_finished (m_cur_event_id))
    return;

  send_changes (false);
}

void document_change_sender::send_changes (bool need_to_clear)
{
  m_cur_event_id = m_queue->add_event (m_items_edit_handler->create_changed_items_event (need_to_clear));
  m_pending_redraw = false;
}