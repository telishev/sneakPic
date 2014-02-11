/// \file main_window.h

#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QMenu>

class Ui_main_window;
class QColor;
class QSettings;
class QSignalMapper;
class QLabel;

class dock_widget_builder;
class settings_t;
class gui_document;
class gui_actions;
class menu_builder;
class style_controller;
class style_widget_handler;
class tools_widget_builder;
class actions_applier;

enum class gui_action_id;


class main_window : public QMainWindow
{
  Q_OBJECT

  Ui_main_window *ui;
  QSettings *m_qsettings;
  QMenu m_recent_menu;
  QSignalMapper *m_signal_mapper;
  QLabel *m_zoom_label;

  settings_t *m_settings;
  gui_document *m_document;
  gui_actions *m_actions;
  actions_applier *m_actions_applier;

  menu_builder *m_menu_builder;
  tools_widget_builder *m_tools_builder;
  dock_widget_builder *m_dock_widget_builder;
  style_widget_handler *m_style_widget_handler;

  std::vector <QString> m_recent_files;
  style_controller *m_style_controller;
  int m_last_saved_position;

public:
  main_window ();
  ~main_window ();
  virtual void closeEvent(QCloseEvent *event);

private slots:
  void zoom_description_changed (const QString &description);
  void open_file (const QString filename);
  bool create_new_document ();
  void update_window_title ();
  void undo_invalidation_check ();

  bool is_doc_saved ();

private:
  void add_file_to_recent (QString file_path);
  void load_recent_menu ();
  void save_recent_menu ();
  void update_recent_menu ();
  QString get_last_file_open_dir () const;
  void create_painter ();
  bool action_triggered (gui_action_id id);
  bool open_file_clicked ();
  bool save_as_clicked ();
  bool save_clicked ();
  void update_on_document_create ();
  void update_undo_position();
  int cur_undo_pos();
  void save_document (QString &filename);
  bool closing_document_check ();

};

#endif // MAIN_WINDOW_H