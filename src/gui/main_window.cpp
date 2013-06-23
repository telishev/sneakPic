/// \file main_window.cpp

#include "main_window.h"

#include "ui/ui_main_window.h"

#include "svg/svg_document.h"

#include <QFileDialog>


main_window::main_window ()
{
  init_clear ();
  ui = new Ui_main_window;
  ui->setupUi (this);

  connect (ui->openFileAct, SIGNAL (triggered ()), this, SLOT (open_file_clicked ()));
  connect (ui->saveFileAct, SIGNAL (triggered ()), this, SLOT (save_file_clicked ()));
}

main_window::~main_window ()
{
  delete ui;
  init_clear ();
}

void main_window::init_clear ()
{
  doc = 0;
}

void main_window::open_file_clicked ()
{
  QString filename = QFileDialog::getOpenFileName (this, "Open File", "C:/Users/alex/svg/test/", "*.svg");
  if (filename.isEmpty ())
    return;

  doc = new svg_document;
  doc->read_file (filename);
}

void main_window::save_file_clicked ()
{
  if (!doc)
    return;

  QString filename = QFileDialog::getSaveFileName (this, "Open File", "C:/Users/alex/svg/test/", "*.svg");
  if (filename.isEmpty ())
    return;

  doc->write_file (filename);
}

