#ifndef PATH_ANCHOR_HANDLE_H
#define PATH_ANCHOR_HANDLE_H

#include "editor/abstract_handle.h"
#include <QPointF>
#include <string>

class QRect;
class QColor;

class svg_item_path;
class svg_path;
class path_handles_editor;

struct single_path_element;

class path_anchor_handle : public abstract_handle
{
  svg_path *m_path;
  svg_item_path *m_item;
  path_handles_editor *m_editor;
  int m_anchor_id;

  QPointF m_drag_start;
  QPointF m_drag_cur;
  bool m_is_highlighted;

public:
  path_anchor_handle (path_handles_editor *editor, svg_item_path *item, int anchor_id, svg_path *path);
  virtual ~path_anchor_handle ();

  int point_id () const { return m_anchor_id; }
  std::string item_name () const;

protected:
  virtual int distance_to_mouse (QPoint screen_pos, QTransform transform) const override;
  virtual void set_mouse_hovered (bool hovered) override;
  virtual bool start_drag (QPointF local_pos) override;
  virtual bool drag (QPointF local_pos) override;
  virtual bool end_drag (QPointF local_pos) override;
  virtual void draw (SkCanvas &canvas, const renderer_state &state, const renderer_config *config) const override;

private:
  QPointF get_handle_center () const;
  QRect get_element_rect (QTransform transform) const;

  void apply_drag ();
  void move_point (svg_path *path);
  QColor current_color () const;
};

#endif // PATH_ANCHOR_HANDLE_H
