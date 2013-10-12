/// \file main_window.h

#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QMenu>

class Ui_main_window;
class QSettings;
class QSignalMapper;
class QLabel;

class settings_t;
class gui_document;
class gui_actions;
class actions_applier;
class menu_builder;

enum class gui_action_id;


class main_window : public QMainWindow
{
  Q_OBJECT

  Ui_main_window *ui;
  QSettings *m_qsettings;
  QMenu m_recent_menu;
  QSignalMapper *m_signal_mapper;
  QLabel *m_zoom_inscription;

  settings_t *m_settings;
  gui_document *m_document;
  gui_actions *m_actions;
  actions_applier *m_applier;
  menu_builder *m_menu_builder;
  
  std::vector <QString> m_recent_files;

public:
  main_window ();
  ~main_window ();

private slots:
  void zoom_description_changed (const QString &description);
  void action_triggered (gui_action_id id);
  void open_file (const QString filename);

private:
  bool open_file_clicked ();
  bool save_file_clicked ();
  void add_file_to_recent (QString file_path);
  void update_window_title ();
  void load_recent_menu ();
  void save_recent_menu ();
  void update_recent_menu ();
  QString get_last_file_open_dir () const;
  void create_painter ();
};


#endif // MAIN_WINDOW_H