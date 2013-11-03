#ifndef RECTANGLE_TOOL_H
#define RECTANGLE_TOOL_H

#include "editor/tools/abstract_tool.h"

#include <QPointF>

class mouse_shortcuts_handler;
class overlay_renderer;
class renderable_item;
class renderer_base_shape_item;
class svg_items_container;

class rectangle_tool : public abstract_tool
{
  renderer_base_shape_item *m_renderer_item;
  svg_items_container      *m_items_container;
  QPointF m_initial_pos;

public:
  rectangle_tool (svg_painter *painter);
  ~rectangle_tool ();

private:
  bool start_rectangle_positioning (const QPoint &pos);
  bool continue_rectangle_positioning (const QPoint &pos);
  bool end_rectangle_positioning (const QPoint &pos);
  void update_preview (QPointF &current_pos);
  void hide_preview();
  void insert_item( const QPointF &pos );
  QRectF get_rect (const QPointF &pos);
};

#endif // RECTANGLE_TOOL_H
