/// \file main_window.cpp

#include "main_window.h"

#include "ui/ui_main_window.h"

#include "common/memory_deallocation.h"

#include "renderer/svg_painter.h"
#include "renderer/rendered_items_cache.h"

#include "svg/svg_document.h"

#include <QFileDialog>
#include <QSettings>
#include <QFileInfo>
#include <QMessageBox>




main_window::main_window ()
{
  init_clear ();
  ui = new Ui_main_window;
  ui->setupUi (this);
  m_settings = new QSettings ("SneakPic");
  m_cache = new rendered_items_cache;
  m_renderer = new svg_painter (ui->glwidget, ui->glwidget->mouse_filter_object (), m_cache);
  ui->glwidget->set_painter (m_renderer);
  update_window_title ();

  connect (ui->openFileAct    , SIGNAL (triggered ()), this, SLOT (open_file_clicked ()));
  connect (ui->saveAsAct    , SIGNAL (triggered ()), this, SLOT (save_file_clicked ()));
  connect (ui->openLastFileAct, SIGNAL (triggered ()), this, SLOT (open_last_file_clicked ()));
}

main_window::~main_window ()
{
  FREE (ui);
  FREE (m_settings);
  FREE (m_renderer);
  FREE (m_cache);
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

  m_settings->setValue ("last_file", filename);
  open_file (filename);
}

void main_window::open_last_file_clicked ()
{
  QString last_file = m_settings->value ("last_file").toString ();
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
  QString last_filename = m_settings->value ("last_file").toString ();
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
  m_renderer->set_document (nullptr);
  FREE (m_doc);
  
  m_doc = new svg_document;
  setWindowTitle ("Loading...");
  if (!m_doc->read_file (filename))
    {
      QMessageBox::warning (this, "Warning", "Cannot open document");
      return;
    }
  
  update_window_title ();
  m_renderer->set_document (m_doc);
  ui->glwidget->repaint ();
}

