#ifndef PATH_PREVIEW_HANDLE_H
#define PATH_PREVIEW_HANDLE_H

#include "editor/abstract_handle.h"

class svg_item_path;
class QRect;
class svg_path_geom;
class svg_painter;
class svg_path_geom_iterator;
class path_edit_operation;
class path_handles_editor;

struct single_path_point;

class path_preview_handle : public abstract_handle
{
  svg_item_path *m_item;
  path_handles_editor *m_editor;
  unique_ptr<path_edit_operation> m_edit_operation;
  unique_ptr<svg_path_geom_iterator> m_drag_it;
  QPointF m_drag_start, m_prev_drag;
  double m_drag_t = -1.0;

public:
  path_preview_handle (path_handles_editor *editor, svg_item_path *item);
  virtual ~path_preview_handle ();

protected:
  virtual int distance_to_mouse (QPoint screen_pos, QTransform transform) const override;
  virtual void set_mouse_hovered (bool hovered) override;
  virtual bool start_drag (QPointF local_pos, QTransform transform) override;
  virtual bool drag (QPointF local_pos, QTransform transform) override;
  virtual bool end_drag (QPointF local_pos, QTransform transform) override;
  virtual void interrupt_drag () override;
  virtual void draw (SkCanvas &canvas, const renderer_state &state, const renderer_config *config) const override;
  virtual handle_priority priority () const override { return handle_priority::LOW; }

private:
  void drag_point (QPointF local_pos);
};

#endif // PATH_PREVIEW_HANDLE_H
