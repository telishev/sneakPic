/// \file main_window.cpp

#include "main_window.h"

#include "ui/ui_main_window.h"

#include "gui/settings.h"

#include "common/common_utils.h"
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
#include <QFileInfo>
#include <QKeyEvent>
#include <QMessageBox>
#include <QSettings>
#include <QTimer>

#define RECENT_FILES_NUMBER 10

main_window::main_window ()
{
  init_clear ();
  ui = new Ui_main_window;
  ui->setupUi (this);
  m_qsettings = new QSettings ("SneakPic");
  m_settings = new settings_t;
  m_cache = new rendered_items_cache;
  m_queue = new events_queue;
  m_painter = new svg_painter (ui->glwidget, ui->glwidget->mouse_filter_object (), m_cache, m_queue, m_settings, this->statusBar ());
  m_renderer_thread = new renderer_thread (new svg_renderer (m_cache, m_queue), m_queue, this);
  m_renderer_thread->start ();
  m_signal_mapper = nullptr;
  ui->glwidget->set_painter (m_painter);
  update_timer = new QTimer (this);
  update_timer->setInterval (50);
  update_timer->start ();

  update_window_title ();  
  ui->openRecentAct->setMenu (&m_recent_menu);
  load_recent_menu ();
  update_recent_menu ();

  connect (ui->openFileAct    , SIGNAL (triggered ()), this, SLOT (open_file_clicked ()));
  connect (ui->saveAsAct      , SIGNAL (triggered ()), this, SLOT (save_file_clicked ()));
  connect (update_timer       , SIGNAL (timeout ())  , this, SLOT (update_timeout ()));
  connect (ui->actionUndo     , SIGNAL (triggered ()), this, SLOT (undo ()));
  connect (ui->actionRedo     , SIGNAL (triggered ()), this, SLOT (redo ()));
}

void main_window::load_recent_menu ()
{
  int size = m_qsettings->beginReadArray ("recent_menu");
  for (int i = 0; i < size; i++)
    {
      m_qsettings->setArrayIndex(i);
      m_recent_files.push_back (m_qsettings->value ("name").toString ());
    }
  m_qsettings->endArray ();
}

void main_window::save_recent_menu ()
{
  m_qsettings->beginWriteArray ("recent_menu", (int) m_recent_files.size ());
  for (unsigned int i = 0; i < m_recent_files.size (); i++)
    {
      m_qsettings->setArrayIndex(i);
      m_qsettings->setValue ("name", m_recent_files[i]);
    }
  m_qsettings->endArray ();
}

void main_window::update_recent_menu ()
{
  m_recent_menu.clear ();
  m_signal_mapper->deleteLater ();
  m_signal_mapper = new QSignalMapper (this);
  int size = (int) m_recent_files.size ();

  for (int i = size - 1; i >= 0; i--)
    {
      QAction *action = m_recent_menu.addAction (QString ("%1.%2")
          .arg (size - i)
          .arg (QFileInfo (m_recent_files[i]).fileName ()), 
        m_signal_mapper, SLOT (map ()), size - i <= 10 ? QKeySequence (Qt::CTRL + Qt::Key_0 + (size - i) % 10) : QKeySequence ());
      m_signal_mapper->setMapping (action, m_recent_files[i]);
    }
   connect(m_signal_mapper, SIGNAL (mapped (QString)), this, SLOT (open_file (QString)));
}

main_window::~main_window ()
{
  save_recent_menu ();
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

  open_file (filename);
}

void main_window::add_file_to_recent (QString file_path)
{
  for (unsigned int i = 0; i < m_recent_files.size (); i++)
    {
      if (file_path == m_recent_files[i])
        {
          m_recent_files.erase (m_recent_files.begin () + i);
          break;
        }
    }
  if (m_recent_files.size () == RECENT_FILES_NUMBER)
    m_recent_files.erase (m_recent_files.begin ());

  m_recent_files.push_back (file_path);
}

void main_window::keyPressEvent(QKeyEvent * qevent)
{
  ui->glwidget->keyPressEvent (qevent);

  if (qevent->isAccepted ())
    return;

  QWidget::keyPressEvent (qevent);
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
  if (m_recent_files.size () == 0)
    return QString ();

  QFileInfo last_file (m_recent_files[0]);
  return last_file.dir ().path ();
}

void main_window::update_window_title ()
{
  setWindowTitle (QString ("sneakPic%1").arg (m_doc ? QString (" - %1").arg (m_doc->get_filename ()) : ""));
}

void main_window::open_file (const QString filename)
{
  m_painter->set_document (nullptr);
  FREE (m_doc);
  DO_ON_EXIT (update_window_title ());

  m_doc = new svg_document (m_settings);
  m_doc->set_queue (m_queue);
  setWindowTitle ("Loading...");
  if (!m_doc->read_file (filename))
    {
      QMessageBox::warning (this, "Warning", "Cannot open document");
      return;
    }

  renderer_items_container *renderer_items = m_doc->create_rendered_items (m_cache);
  m_queue->add_event (new event_container_changed (renderer_items));
  m_painter->set_document (m_doc);
  add_file_to_recent (filename);
  update_recent_menu ();
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

void main_window::undo ()
{
  if (m_doc)
    m_doc->undo ();
}

void main_window::redo ()
{
  if (m_doc)
    m_doc->redo ();
}
