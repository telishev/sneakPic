#ifndef PATH_CONTROL_POINT_HANDLE_H
#define PATH_CONTROL_POINT_HANDLE_H


#include "editor/abstract_handle.h"
#include "path/svg_path_geom.h"
#include <QPointF>

class QRect;
class QColor;

class svg_item_path;
class svg_path_geom;
class path_handles_editor;
class path_edit_operation;

struct single_path_point;

class path_control_point_handle : public abstract_handle
{
  svg_item_path *m_item;
  path_handles_editor *m_editor;
  svg_path_geom_iterator m_path_it;
  bool m_left_handle;

  QPointF m_drag_start;
  QPointF m_drag_cur;
  bool m_is_highlighted;
  std::unique_ptr<path_edit_operation> m_edit_operation;

public:
  path_control_point_handle (path_handles_editor *editor, svg_item_path *item, svg_path_geom_iterator path_it, bool is_left_handle);
  virtual ~path_control_point_handle ();

protected:
  virtual int distance_to_mouse (QPoint screen_pos, QTransform transform) const override;
  virtual void set_mouse_hovered (bool hovered) override;
  virtual bool start_drag (QPointF local_pos) override;
  virtual bool drag (QPointF local_pos) override;
  virtual bool end_drag (QPointF local_pos) override;
  virtual void draw (SkCanvas &canvas, const renderer_state &state, const renderer_config *config) const override;

private:
  QPointF get_handle_center () const;
  QPointF get_anchor_center () const;

  void apply_drag ();
  void move_point ();

  const svg_path_geom *get_path () const;
};


#endif // PATH_CONTROL_POINT_HANDLE_H
