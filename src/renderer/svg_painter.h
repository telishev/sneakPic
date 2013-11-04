#ifndef SVG_PAINTER_H
#define SVG_PAINTER_H

#include "renderer/abstract_painter.h"

#include <QLabel>
#include <QTransform>

class svg_document;
class abstract_svg_item;
class rendered_items_cache;
class events_queue;
class overlay_renderer;
class items_selection;
class mouse_shortcuts_handler;
class settings_t;
class current_item_outline_renderer;
class rubberband_selection;
class abstract_tool;
class svg_items_container;
class items_selection_renderer;
class actions_applier;

enum class gui_action_id;

class svg_painter : public abstract_painter
{
  Q_OBJECT

  QTransform m_cur_transform;
  QTransform m_last_transform;
  QPoint m_drag_start_pos;

  svg_document *m_document;
  rendered_items_cache *m_cache;
  events_queue *m_queue;
  overlay_renderer *m_overlay;
  items_selection *m_selection;
  items_selection_renderer *m_selection_renderer;
  current_item_outline_renderer *m_item_outline;
  mouse_shortcuts_handler *m_mouse_handler;
  settings_t *m_settings;
  actions_applier *m_actions_applier;

  abstract_tool *m_current_tool;

public:
  svg_painter (gl_widget *glwidget, rendered_items_cache *cache, events_queue *queue, svg_document *document, settings_t *settings);
  ~svg_painter ();

  void set_current_tool (abstract_tool *tool);

  settings_t *settings () const { return m_settings; }
  items_selection *selection () const { return m_selection; }
  svg_document *document () const { return m_document; }
  svg_items_container *item_container () const;
  QTransform cur_transform () const { return m_cur_transform; }

  abstract_svg_item *get_current_item (const QPoint &pos);
  QPointF get_local_pos (const QPointF &mouse_pos) const;
  void update_status_bar_widgets ();

  void redraw ();
  bool action_triggered (gui_action_id id);

signals:
  void zoom_description_changed (const QString &description);

private slots:
  void items_changed ();
  void selection_changed ();

protected:
  virtual unsigned int mouse_event (const mouse_event_t &m_event) override;
  virtual void draw () override;
  virtual void configure () override;

  virtual void wheelEvent (QWheelEvent *qevent) override;
  virtual void leaveEvent (QEvent *qevent) override;
  virtual void resizeEvent (QResizeEvent *qevent) override;
  virtual bool event (QEvent * /*qevent*/) override { return false; }

private:
  void set_document (svg_document *document);
  void reset_transform ();
  void send_changes (bool interrrupt_rendering);
  void update_drawing (QTransform transform);
  bool select_item (const QPoint &pos, bool clear_selection);

  mouse_shortcuts_handler *create_mouse_shortcuts ();
  bool start_pan (const QPoint &pos);
  bool pan_picture (const QPoint &pos);
  bool find_current_object (const QPoint &pos);
  void create_overlay_containers ();
  abstract_svg_item *get_current_item_for_point (const QPoint &pos);
  bool remove_items_in_selection ();
};

#endif // SVG_PAINTER_H


