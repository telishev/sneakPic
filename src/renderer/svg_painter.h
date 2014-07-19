#ifndef SVG_PAINTER_H
#define SVG_PAINTER_H

#include "renderer/abstract_painter.h"

#include <QLabel>
#include <QTransform>

class abstract_svg_item;
class abstract_tool;
class actions_applier;
class current_item_outline_renderer;
class events_queue;
class items_selection;
class items_selection_renderer;
class mouse_shortcut_enum_union;
class mouse_shortcuts_handler;
class overlay_renderer;
class rendered_items_cache;
class renderer_overlay_path;
class renderer_page;
class rubberband_selection;
class settings_t;
class svg_document;
class svg_items_container;
class style_controller;
class gui_document;
class selection_actions;

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
  renderer_page *m_renderer_page;
  gui_document *m_gui_document;

  abstract_tool *m_current_tool;

  QPointF m_color_picker_pos;
  unique_ptr<renderer_overlay_path> m_color_picker_area_preview;
  unique_ptr<selection_actions> m_selection_actions;

public:
  svg_painter (canvas_widget_t *canvas_widget, rendered_items_cache *cache, events_queue *queue, svg_document *document, settings_t *settings, gui_document *gui_doc);
  ~svg_painter ();

  void set_current_tool (abstract_tool *tool);

  settings_t *settings () const { return m_settings; }
  items_selection *selection () const { return m_selection; }
  svg_document *document () const { return m_document; }
  svg_items_container *item_container () const;
  QTransform cur_transform () const { return m_cur_transform; }
  style_controller *get_style_controller () const;
  gui_document *get_gui_document () const { return m_gui_document; }
  actions_applier *get_actions_applier () const { return m_actions_applier; }

  abstract_svg_item *get_current_item (const QPoint &pos);
  QPointF get_local_pos (const QPointF &mouse_pos) const;
  void update_status_bar_widgets ();

  void redraw ();
  bool action_triggered (gui_action_id id);
signals:
  void zoom_description_changed (const QString &description);
  void color_picked (const QColor &color);
  void selection_changed_signal ();

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

  bool start_pan (const QPoint &pos);
  bool pan_picture (const QPoint &pos);
  bool end_pan (const QPoint &/*pos*/) { return true; }
  bool find_current_object (const QPoint &pos);
  void create_overlay_containers ();
  abstract_svg_item *get_current_item_for_point (const QPoint &pos);
  bool process_mouse_event (const mouse_event_t &event, mouse_shortcut_enum_union action);
  bool pick_color_start (const QPoint &pos);
  bool pick_color_drag (const QPoint &pos);
  bool pick_color_end (const QPoint &pos);
  bool pick_color_click (const QPoint &pos);
};

#endif // SVG_PAINTER_H



