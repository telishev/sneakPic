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
class layers_handler;
class layers_widget_handler;
class menu_builder;
class tool_properties_widget;
class style_controller;
class style_widget_handler;
class tools_widget_builder;
class actions_applier;

enum class gui_action_id;

class options_dialog;

class main_window : public QMainWindow
{
  Q_OBJECT

  Ui_main_window *ui;
  QSettings *m_qsettings;
  QMenu m_recent_menu;
  unique_ptr <QSignalMapper> m_recent_files_signal_mapper;
  QLabel *m_zoom_label;

  unique_ptr <settings_t> m_settings;
  gui_document *m_document;
  gui_actions *m_actions;
  actions_applier *m_actions_applier;

  menu_builder *m_menu_builder;
  tools_widget_builder *m_tools_builder;
  dock_widget_builder *m_dock_widget_builder;
  style_widget_handler *m_style_widget_handler;
  unique_ptr <layers_widget_handler> m_layers_widget_handler;
  unique_ptr <layers_handler> m_layers_handler;
  unique_ptr <tool_properties_widget> m_tool_properties_widget;

  std::deque <QString> m_recent_files;
  style_controller *m_style_controller;
  unique_ptr <options_dialog> m_options_dialog;
  int m_last_saved_position;

public:
  main_window ();
  ~main_window ();
  virtual void closeEvent(QCloseEvent *event);

private slots:
  void zoom_description_changed (const QString &description);
  void do_open_file (const QString filename);
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
  bool open_file ();
  bool save_as ();
  bool save ();
  void update_on_document_create ();
  void update_undo_position();
  int cur_undo_pos();
  void save_document (const QString& filename);
  bool closing_document_check ();
  bool options ();
  bool export_bitmap ();
  };

#endif // MAIN_WINDOW_H
