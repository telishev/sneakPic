#ifndef ELLIPSE_TOOL_H
#define ELLIPSE_TOOL_H

#include "editor/tools/abstract_tool.h"

#include <QPointF>

class renderer_overlay_path;
class svg_items_container;

class ellipse_tool : public abstract_tool
{
  renderer_overlay_path *m_renderer_item;
  svg_items_container      *m_items_container;
  QPointF m_initial_pos;

public:
  ellipse_tool (svg_painter *painter);
  ~ellipse_tool ();

private:
  bool start_ellipse_positioning (const QPoint &pos);
  bool continue_ellipse_positioning (const QPoint &pos);
  bool end_ellipse_positioning (const QPoint &pos);
  void update_preview (QPointF &current_pos);
  void hide_preview();
  void insert_item( const QPointF &pos );
  QRectF get_rect (const QPointF &pos);
};

#endif // ELLIPSE_TOOL_H
