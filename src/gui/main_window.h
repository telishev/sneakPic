/// \file main_window.h

#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>

class Ui_main_window;

class svg_document;

class main_window : public QMainWindow
{
  Q_OBJECT

  Ui_main_window *ui;
  svg_document *doc;

public:
  main_window ();
  ~main_window ();

private slots:
  void open_file_clicked ();
  void save_file_clicked ();

private:
  void init_clear ();
};


#endif // MAIN_WINDOW_H