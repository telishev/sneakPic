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
#include "gui/options_dialog.h"

#include "common/common_utils.h"
#include "common/memory_deallocation.h"

#include "svg/svg_document.h"
#include "svg/undo/undo_handler.h"

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
  m_settings.reset (new settings_t (m_qsettings));
  m_settings->load ();
  m_actions = new gui_actions (m_settings->shortcuts_cfg (), [&] (gui_action_id id) { return action_triggered (id); }, this);
  m_dock_widget_builder = new dock_widget_builder (this);
  m_tools_builder = new tools_widget_builder (m_actions, m_dock_widget_builder);
  m_style_controller = new style_controller (m_settings.get ());
  m_style_widget_handler = new style_widget_handler (m_dock_widget_builder, m_style_controller);
  m_last_saved_position = 0;
  m_recent_files_signal_mapper.reset (new QSignalMapper ());
  m_options_dialog.reset (new options_dialog (m_settings.get ()));
  CONNECT (m_recent_files_signal_mapper.get (), (void (QSignalMapper::*) (const QString&)) &QSignalMapper::mapped, this, &main_window::do_open_file);

  m_menu_builder = new menu_builder (menuBar (), m_actions, createPopupMenu ());

  CONNECT (m_style_widget_handler, &style_widget_handler::stroke_color_changed, m_tools_builder, &tools_widget_builder::update_stroke_color);
  CONNECT (m_style_widget_handler, &style_widget_handler::fill_color_changed, m_tools_builder, &tools_widget_builder::update_fill_color);

  update_window_title ();
  m_actions->action (gui_action_id::OPEN_RECENT)->setMenu (&m_recent_menu);
  load_recent_menu ();
  update_recent_menu ();
  m_zoom_label = new QLabel;
  statusBar ()->addPermanentWidget (m_zoom_label);
  restoreGeometry (m_qsettings->value ("main_window_geometry").toByteArray ());
  restoreState (m_qsettings->value ("main_window_state").toByteArray ());

  m_actions_applier = new actions_applier;
  m_actions_applier->register_action (gui_action_id::NEW, this, &main_window::create_new_document);
  m_actions_applier->register_action (gui_action_id::OPEN, this, &main_window::open_file);
  m_actions_applier->register_action (gui_action_id::SAVE, this, &main_window::save);
  m_actions_applier->register_action (gui_action_id::SAVE_AS, this, &main_window::save_as);
  m_actions_applier->register_action (gui_action_id::OPTIONS, this, &main_window::options);
  m_actions_applier->register_action (gui_action_id::QUIT, (QWidget *)this, &QWidget::close);


  QTimer::singleShot (10, this, SLOT (create_new_document ()));
}

main_window::~main_window ()
{
  m_qsettings->setValue ("main_window_geometry", this->saveGeometry ());
  m_qsettings->setValue ("main_window_state", this->saveState ());
  m_settings->save ();
  save_recent_menu ();
  FREE (m_actions);
  FREE (ui);
  FREE (m_qsettings);
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
  int size = (int) m_recent_files.size ();

  for (int i = 0; i < size; i++)
    {
      int num = (i + 1) % 10;
      QAction *action = m_recent_menu.addAction (QString ("%1.%2")
                                                 .arg (num)
                                                 .arg (QFileInfo (m_recent_files[i]).fileName ()),
                                                 m_recent_files_signal_mapper.get (), SLOT (map ()), i <= 10 ? QKeySequence (Qt::CTRL + Qt::Key_0 + num) : QKeySequence ());
      m_recent_files_signal_mapper->setMapping (action, m_recent_files[i]);
    }
}


bool main_window::options ()
{
  m_options_dialog->init ();
  m_options_dialog->show ();
  return true;
}

bool main_window::open_file ()
{
  QString filename = QFileDialog::getOpenFileName (this, "Open File", get_last_file_open_dir (), "Scalable Vector Graphics (*.svg)");
  if (filename.isEmpty ())
    return true;

  do_open_file (filename);
  return true;
}

bool main_window::save_as ()
{
  if (!m_document)
    return true;

  QString filename = QFileDialog::getSaveFileName (this, "Save File", !m_document->is_new_document () ? m_document->get_filename () : get_last_file_open_dir (), "Scalable Vector Graphics (*.svg)");
  if (filename.isEmpty ())
    return false;

  save_document (filename);
  return true;
}

void main_window::save_document (QString &filename)
{
  if (!m_document->save_file (filename))
    return;
  update_undo_position ();
  update_window_title ();
  add_file_to_recent (filename);
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

  m_recent_files.push_front (file_path);

  update_recent_menu ();
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
  QString title = QString ("sneakPic");
  if (m_document && !m_document->get_filename ().isEmpty ())
    {
      title.prepend (" - ");
      title.prepend (m_document->get_filename ());
    }

  if (!is_doc_saved ())
    title.prepend ('*');
  if (windowTitle () != title) // kinda superstitious protection
    setWindowTitle (title);
}

void main_window::do_open_file (const QString filename)
{
  if (!closing_document_check ())
    return;

  FREE (m_document);
  DO_ON_EXIT (update_window_title ());

  m_document = new gui_document (m_settings.get (), m_actions);
  setWindowTitle ("Loading...");
  if (!m_document->open_file (filename))
    {
      create_new_document ();
      QMessageBox::warning (this, "Warning", "Cannot open document");
      return;
    }

  m_last_saved_position = 0;
  update_on_document_create ();
  add_file_to_recent (filename);
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

void main_window::update_on_document_create ()
{
  create_painter ();
  m_style_widget_handler->set_tools_containter (m_document->get_tools_container ());
  CONNECT (m_document->doc (), &svg_document::undo_redo_done, this, &main_window::update_window_title);
  CONNECT (m_document->doc (), &svg_document::changes_done, this, &main_window::undo_invalidation_check);
}

void main_window::undo_invalidation_check ()
{
  if (m_last_saved_position >= cur_undo_pos ())
    m_last_saved_position = -1;
  update_window_title ();
}

bool main_window::create_new_document ()
{
  if (!closing_document_check ())
    return false;

  FREE (m_document);
  DO_ON_EXIT (update_window_title ());
  m_document = new gui_document (m_settings.get (), m_actions);
  if (!m_document->create_new_document ())
    return false;

  update_on_document_create ();
  update_window_title ();
  ui->glwidget->repaint ();
  return true;
}

void main_window::update_undo_position ()
{
  m_last_saved_position = cur_undo_pos();
}

int main_window::cur_undo_pos ()
{
  return m_document ? m_document->doc ()->get_undo_handler ()->cur_undo_position () : 0;
}

bool main_window::is_doc_saved ()
{
  return m_last_saved_position == cur_undo_pos ();
}

bool main_window::closing_document_check ()
{
  if (!is_doc_saved ())
  {
    auto result = QMessageBox::question (this, "sneakPic", "Save changed file '" + QFileInfo (m_document->get_filename ()).fileName () + "'?", QMessageBox::Yes, QMessageBox::No, QMessageBox::Cancel);
    if (result == QMessageBox::Cancel)
    {
      return false;
    }

    if (result == QMessageBox::Yes)
    {
      if (!save ())
      {
        return false;
      }
    }

    return true;
  }
  else
    return true;
}

void main_window::closeEvent (QCloseEvent *event)
{
  if (closing_document_check ())
    event->accept ();
  else
    event->ignore ();
}

bool main_window::save ()
{
  if (m_document->is_new_document ())
    return save_as ();
  else
    save_document (m_document->get_filename ());
  return true;
}
