#ifndef GUI_DOCUMENT_H
#define GUI_DOCUMENT_H

#include <QObject>
#include <QString>

#include <functional>

class QTimer;
class settings_t;
class rendered_items_cache;
class renderer_thread;
class events_queue;
class svg_document;
class svg_painter;
class tools_container;
class gl_widget;
class gui_actions;
class actions_applier;
class copy_paste_handler;

enum class gui_action_id;

class gui_document : public QObject
{
  Q_OBJECT
  settings_t  *m_settings;
  gui_actions *m_actions;

  QTimer *update_timer;
  rendered_items_cache *m_cache;
  renderer_thread      *m_renderer_thread;
  events_queue         *m_queue;

  svg_document *m_doc;
  svg_painter  *m_painter;
  tools_container *m_tools_container;
  actions_applier *m_actions_applier;
  unique_ptr<copy_paste_handler> m_copy_paste_handler;

public:
  gui_document (settings_t *settings, gui_actions *actions);
  ~gui_document ();

  svg_painter *create_painter (gl_widget *widget);
  svg_painter *painter () const { return m_painter; }

  bool open_file (const QString &filename);
  bool save_file (const QString &filename);

  bool is_new_document ();
  bool create_new_document ();

  QString get_filename () const;
  bool action_triggered (gui_action_id id);

  const tools_container *get_tools_container () { return m_tools_container; }
  const svg_document* doc () const { return m_doc; }

private slots:
  void update_timeout ();
  void tool_changed ();

private:
  bool undo ();
  bool redo ();
  bool create_new_document_impl (std::function <bool (svg_document *)> create_func);
  bool paste ();
  bool copy ();
  bool cut ();
};


#endif // GUI_DOCUMENT_H
