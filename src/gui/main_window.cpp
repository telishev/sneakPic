/// \file main_window.cpp

#include "main_window.h"

#include "ui/ui_main_window.h"

#include "common/memory_deallocation.h"

#include "renderer/svg_renderer.h"

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
  settings = new QSettings ("SneakPic");
  renderer = new svg_renderer (ui->glwidget, ui->glwidget->mouse_filter_object ());
  ui->glwidget->set_painter (renderer);

  connect (ui->openFileAct    , SIGNAL (triggered ()), this, SLOT (open_file_clicked ()));
  connect (ui->saveFileAct    , SIGNAL (triggered ()), this, SLOT (save_file_clicked ()));
  connect (ui->openLastFileAct, SIGNAL (triggered ()), this, SLOT (open_last_file_clicked ()));
}

main_window::~main_window ()
{
  FREE (ui);
  FREE (settings);
  init_clear ();
}

void main_window::init_clear ()
{
  doc = nullptr;
}

void main_window::open_file_clicked ()
{
  QString filename = QFileDialog::getOpenFileName (this, "Open File", get_last_file_open_dir (), "*.svg");
  if (filename.isEmpty ())
    return;

  settings->setValue ("last_file", filename);
  open_file (filename);
}

void main_window::open_last_file_clicked ()
{
  QString last_file = settings->value ("last_file").toString ();
  if (last_file.isEmpty ())
    return;

  open_file (last_file);
}


void main_window::save_file_clicked ()
{
  if (!doc)
    return;

  QString filename = QFileDialog::getSaveFileName (this, "Open File", get_last_file_open_dir (), "*.svg");
  if (filename.isEmpty ())
    return;

  doc->write_file (filename);
}

QString main_window::get_last_file_open_dir () const
{
  QString last_filename = settings->value ("last_file").toString ();
  if (last_filename.isEmpty ())
    return QString ();

  QFileInfo last_file (last_filename);
  return last_file.dir ().path ();
}

void main_window::open_file (const QString &filename)
{
  renderer->set_document (nullptr);
  FREE (doc);
  
  doc = new svg_document;
  if (!doc->read_file (filename))
    {
      QMessageBox::warning (this, "Warning", "Cannot open document");
      return;
    }
  renderer->set_document (doc);
  ui->glwidget->repaint ();
}

