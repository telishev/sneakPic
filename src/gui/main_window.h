/// \file main_window.h

#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QMenu>

class Ui_main_window;
class QSettings;
class QSignalMapper;
class QLabel;

class svg_document;
class svg_painter;
class rendered_items_cache;
class renderer_thread;
class events_queue;
class settings_t;
class tools_container;


class main_window : public QMainWindow
{
  Q_OBJECT

  Ui_main_window *ui;
  QSettings *m_qsettings;
  QMenu m_recent_menu;
  QSignalMapper *m_signal_mapper;
  QTimer *update_timer;
  QLabel *m_zoom_inscription;
  
  settings_t           *m_settings;
  rendered_items_cache *m_cache;
  renderer_thread      *m_renderer_thread;
  events_queue         *m_queue;

  svg_document *m_doc;
  svg_painter  *m_painter;
  tools_container *m_tools_container;

  std::vector <QString> m_recent_files;

public:
  main_window ();
  ~main_window ();

private slots:
  void open_file_clicked ();
  void save_file_clicked ();
  void update_timeout ();
  void open_file (const QString filename);
  void undo ();
  void redo ();
  void zoom_description_changed (const QString &description);

private:
  void add_file_to_recent (QString file_path);
  void keyPressEvent(QKeyEvent * qevent);
  void update_window_title ();
  void load_recent_menu ();
  void save_recent_menu ();
  void update_recent_menu ();
  void init_clear (); 
  QString get_last_file_open_dir () const;
  void create_painter (svg_document *doc);
};


#endif // MAIN_WINDOW_H