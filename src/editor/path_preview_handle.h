#ifndef PATH_PREVIEW_HANDLE_H
#define PATH_PREVIEW_HANDLE_H

#include "editor/abstract_handle.h"

class svg_item_path;
class QRect;
class svg_path;
class svg_painter;

struct single_path_element;

class path_preview_handle : public abstract_handle
{
  svg_path *m_path;
  svg_item_path *m_item;
  svg_painter *m_painter;

public:
  path_preview_handle (svg_painter *painter, svg_item_path *item, svg_path *path);
  virtual ~path_preview_handle ();

protected:
  virtual bool is_mouse_inside (QPoint screen_pos, QTransform transform) const override;
  virtual void set_mouse_hovered (bool hovered) override;
  virtual bool start_drag (QPointF local_pos) override;
  virtual bool drag (QPointF local_pos) override;
  virtual bool end_drag (QPointF local_pos) override;
  virtual void draw (SkCanvas &canvas, const renderer_state &state, const renderer_config *config) const override;

private:
};

#endif // PATH_PREVIEW_HANDLE_H