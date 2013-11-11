/// \file main_window.cpp

#include "gui/main_window.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QKeyEvent>
#include <QMessageBox>
#include <QSettings>
#include <QTimer>
#include <QLabel>
#include <QMenuBar>

#include <ui_main_window.h>

#include "editor/style_controller.h"

#include "gui/settings.h"
#include "gui/gui_document.h"
#include "gui/gui_actions.h"
#include "gui/gui_action_id.h"
#include "gui/style_widget_handler.h"
#include "gui/tools_widget_builder.h"
#include "gui/menu_builder.h"
#include "gui/connection.h"

#include "common/common_utils.h"
#include "common/memory_deallocation.h"

#include "svg/svg_document.h"

#include "dock_widget_builder.h"
#include "renderer/svg_painter.h"
#include "actions_applier.h"

#define RECENT_FILES_NUMBER 10

main_window::main_window ()
{
  ui = new Ui_main_window;
  ui->setupUi (this);
  m_document = nullptr;
  m_qsettings = new QSettings ("SneakPic");
  m_settings = new settings_t;
  m_signal_mapper = nullptr;
  m_actions = new gui_actions (m_settings->shortcuts_cfg (), [&] (gui_action_id id) { return action_triggered (id); }, this);
  m_dock_widget_builder = new dock_widget_builder (this);
  m_tools_builder = new tools_widget_builder (m_actions, m_dock_widget_builder);
  m_style_controller = new style_controller (m_settings);
  m_style_widget_handler = new style_widget_handler (m_dock_widget_builder, m_style_controller);


  m_menu_builder = new menu_builder (menuBar (), m_actions, createPopupMenu ());

  update_window_title ();
  m_actions->action (gui_action_id::OPEN_RECENT)->setMenu (&m_recent_menu);
  load_recent_menu ();
  update_recent_menu ();
  m_zoom_label = new QLabel;
  statusBar ()->addPermanentWidget (m_zoom_label);
  this->restoreGeometry (m_qsettings->value ("main_window_geometry").toByteArray ());
  this->restoreState (m_qsettings->value ("main_window_state").toByteArray ());

  m_actions_applier = new actions_applier;
  m_actions_applier->register_action (gui_action_id::OPEN, this, &main_window::open_file_clicked);
  m_actions_applier->register_action (gui_action_id::SAVE_AS, this, &main_window::save_file_clicked);
  m_actions_applier->register_action (gui_action_id::QUIT, (QWidget *)this, &QWidget::close);

}

main_window::~main_window ()
{
  m_qsettings->setValue ("main_window_geometry", this->saveGeometry ());
  m_qsettings->setValue ("main_window_state", this->saveState ());
  save_recent_menu ();
  FREE (m_actions);
  FREE (ui);
  FREE (m_qsettings);
  FREE (m_settings);
  FREE (m_dock_widget_builder);
  FREE (m_tools_builder);
  FREE (m_menu_builder);
  FREE (m_document);
  FREE (m_actions_applier);
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
  if (m_signal_mapper)
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
  CONNECT (m_signal_mapper, (void (QSignalMapper::*) (const QString&)) &QSignalMapper::mapped, this, &main_window::open_file);
}


bool main_window::open_file_clicked ()
{
  QString filename = QFileDialog::getOpenFileName (this, "Open File", get_last_file_open_dir (), "Scalable Vector Graphics (*.svg)");
  if (filename.isEmpty ())
    return true;

  open_file (filename);
  return true;
}

bool main_window::save_file_clicked ()
{
  if (!m_document)
    return true;

  QString filename = QFileDialog::getSaveFileName (this, "Save File", m_document->get_filename (), "Scalable Vector Graphics (*.svg)");
  if (filename.isEmpty ())
    return true;

  m_document->save_file (filename);
  update_window_title ();
  return true;
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

QString main_window::get_last_file_open_dir () const
{
  if (m_recent_files.size () == 0)
    return QString ();

  QFileInfo last_file (m_recent_files[0]);
  return last_file.dir ().path ();
}

void main_window::update_window_title ()
{
  setWindowTitle (QString ("sneakPic%1").arg (m_document ? QString (" - %1").arg (m_document->get_filename ()) : ""));
}

void main_window::open_file (const QString filename)
{
  FREE (m_document);
  DO_ON_EXIT (update_window_title ());

  m_document = new gui_document (m_settings, m_actions);
  setWindowTitle ("Loading...");
  if (!m_document->open_file (filename))
    {
      QMessageBox::warning (this, "Warning", "Cannot open document");
      return;
    }

  create_painter ();
  add_file_to_recent (filename);
  update_recent_menu ();
  ui->glwidget->repaint ();
}

void main_window::zoom_description_changed (const QString &description)
{
  m_zoom_label->setText (description);
}

void main_window::create_painter ()
{
  svg_painter *painter = m_document->create_painter (ui->glwidget);
  CONNECT (painter, &svg_painter::zoom_description_changed, this, &main_window::zoom_description_changed);
  m_style_controller->set_painter (painter);
  painter->update_status_bar_widgets ();
}

bool main_window::action_triggered (gui_action_id id)
{
  if (m_document)
    if (m_document->action_triggered (id))
      return true;

  return m_actions_applier->apply_action (id);
}

