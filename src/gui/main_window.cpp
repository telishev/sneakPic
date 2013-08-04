/// \file main_window.cpp

#include "main_window.h"

#include "ui/ui_main_window.h"

#include "gui/settings.h"

#include "common/memory_deallocation.h"

#include "renderer/svg_painter.h"
#include "renderer/rendered_items_cache.h"
#include "renderer/abstract_renderer_event.h"
#include "renderer/renderer_thread.h"
#include "renderer/svg_renderer.h"
#include "renderer/event_container_changed.h"
#include "renderer/events_queue.h"

#include "svg/svg_document.h"

#include <QFileDialog>
#include <QSettings>
#include <QFileInfo>
#include <QMessageBox>
#include <QTimer>


main_window::main_window ()
{
  init_clear ();
  ui = new Ui_main_window;
  ui->setupUi (this);
  m_qsettings = new QSettings ("SneakPic");
  m_settings = new settings_t;
  m_cache = new rendered_items_cache;
  m_queue = new events_queue;
  m_painter = new svg_painter (ui->glwidget, ui->glwidget->mouse_filter_object (), m_cache, m_queue, m_settings);
  m_renderer_thread = new renderer_thread (new svg_renderer (m_cache, m_queue), m_queue, this);
  m_renderer_thread->start ();
  ui->glwidget->set_painter (m_painter);
  update_timer = new QTimer (this);
  update_timer->setInterval (50);
  update_timer->start ();

  update_window_title ();

  connect (ui->openFileAct    , SIGNAL (triggered ()), this, SLOT (open_file_clicked ()));
  connect (ui->saveAsAct      , SIGNAL (triggered ()), this, SLOT (save_file_clicked ()));
  connect (ui->openLastFileAct, SIGNAL (triggered ()), this, SLOT (open_last_file_clicked ()));
  connect (update_timer       , SIGNAL (timeout ())  , this, SLOT (update_timeout ()));
}

main_window::~main_window ()
{
  m_renderer_thread->set_exit_needed ();
  m_renderer_thread->wait ();
  FREE (m_renderer_thread);
  FREE (m_queue);
  FREE (ui);
  FREE (m_qsettings);
  FREE (m_painter);
  FREE (m_cache);
  FREE (m_doc);
  FREE (m_settings);
  init_clear ();
}

void main_window::init_clear ()
{
  m_doc = nullptr;
}

void main_window::open_file_clicked ()
{
  QString filename = QFileDialog::getOpenFileName (this, "Open File", get_last_file_open_dir (), "Scalable Vector Graphics (*.svg)");
  if (filename.isEmpty ())
    return;

  m_qsettings->setValue ("last_file", filename);
  open_file (filename);
}

void main_window::open_last_file_clicked ()
{
  QString last_file = m_qsettings->value ("last_file").toString ();
  if (last_file.isEmpty ())
    return;

  open_file (last_file);
}


void main_window::save_file_clicked ()
{
  if (!m_doc)
    return;

  QString filename = QFileDialog::getSaveFileName (this, "Save File", m_doc->get_filename (), "Scalable Vector Graphics (*.svg)");
  if (filename.isEmpty ())
    return;

  m_doc->write_file (filename);
  update_window_title ();
}

QString main_window::get_last_file_open_dir () const
{
  QString last_filename = m_qsettings->value ("last_file").toString ();
  if (last_filename.isEmpty ())
    return QString ();

  QFileInfo last_file (last_filename);
  return last_file.dir ().path ();
}

void main_window::update_window_title ()
{
  setWindowTitle (QString ("sneakPic%1").arg (m_doc ? QString (" - %1").arg (m_doc->get_filename ()) : ""));
}

void main_window::open_file (const QString &filename)
{
  m_painter->set_document (nullptr);
  FREE (m_doc);

  m_doc = new svg_document (m_settings);
  setWindowTitle ("Loading...");
  if (!m_doc->read_file (filename))
    {
      QMessageBox::warning (this, "Warning", "Cannot open document");
      return;
    }

  renderer_items_container *renderer_items = m_doc->create_rendered_items (m_cache);
  m_queue->add_event (new event_container_changed (renderer_items));
  update_window_title ();
  m_painter->set_document (m_doc);
  ui->glwidget->repaint ();
}

void main_window::update_timeout ()
{
  if (m_cache->has_pending_changes ())
    {
      m_painter->set_configure_needed (CONFIGURE_TYPE__REDRAW, 1);
      ui->glwidget->repaint ();
    }
}
