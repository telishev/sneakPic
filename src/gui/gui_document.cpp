#include "gui/gui_document.h"

#include <QTimer>

#include "common/memory_deallocation.h"

#include "editor/tools/tools_container.h"

#include "gui/gl_widget.h"
#include "gui/actions_applier.h"
#include "gui/gui_action_id.h"

#include "renderer/rendered_items_cache.h"
#include "renderer/events_queue.h"
#include "renderer/renderer_thread.h"
#include "renderer/svg_renderer.h"
#include "renderer/svg_painter.h"
#include "renderer/event_container_changed.h"

#include "svg/svg_document.h"


gui_document::gui_document (settings_t *settings)
{
  m_painter = nullptr;
  m_settings = settings;
  m_cache = new rendered_items_cache;
  m_queue = new events_queue;
  m_tools_container = new tools_container;
  m_applier = new actions_applier;
  m_renderer_thread = new renderer_thread (new svg_renderer (m_cache, m_queue), m_queue);
  m_renderer_thread->start ();

  update_timer = new QTimer (this);
  update_timer->setInterval (50);
  update_timer->start ();
  connect (update_timer, SIGNAL (timeout ()), this, SLOT (update_timeout ()));

  m_applier->register_action (gui_action_id::UNDO, [&] () { return undo (); });
  m_applier->register_action (gui_action_id::REDO, [&] () { return redo (); });
}

gui_document::~gui_document ()
{
  m_renderer_thread->set_exit_needed ();
  m_renderer_thread->wait ();
  FREE (m_renderer_thread);
  FREE (m_queue);
  FREE (m_tools_container);
  FREE (m_painter);
  FREE (m_cache);
  FREE (m_doc);
  FREE (m_applier);
}

bool gui_document::open_file (const QString &filename)
{
  m_doc = new svg_document ();
  m_doc->set_queue (m_queue);
  if (!m_doc->read_file (filename))
    {
      return false;
    }

  renderer_items_container *renderer_items = m_doc->create_rendered_items (m_cache);
  m_queue->add_event (new event_container_changed (renderer_items));
  return true;
}

bool gui_document::save_file (const QString &filename)
{
  return m_doc->write_file (filename);
}

svg_painter *gui_document::create_painter (gl_widget *widget)
{
  FREE (m_painter);
  m_painter = new svg_painter (widget, m_cache, m_queue, m_doc, m_settings);
  m_tools_container->update_tools (m_painter);
  m_painter->set_current_tool (m_tools_container->current_tool ());
  widget->set_painter (m_painter);
  return m_painter;
}

QString gui_document::get_filename () const
{
  return m_doc ? m_doc->get_filename () : QString ();
}

void gui_document::update_timeout ()
{
  if (m_cache->has_pending_changes ())
    m_painter->update ();
}

bool gui_document::action_triggered (gui_action_id id)
{
  if (m_applier->apply_action (id))
    return true;

  return false;
}

bool gui_document::undo ()
{
  m_doc->undo ();
  return true;
}

bool gui_document::redo ()
{
  m_doc->redo ();
  return true;
}
