#ifndef ITEMS_MOVE_HANDLER_H
#define ITEMS_MOVE_HANDLER_H

#include <QPointF>
#include <string>

class QTransform;

class transformed_renderable_items;
class overlay_renderer;
class items_selection;
class svg_document;
class svg_items_container;

class items_move_handler
{
  QPointF m_start_pos, m_cur_pos;
  overlay_renderer *m_overlay_renderer;
  items_selection *m_selection;
  svg_document *m_document;
  svg_items_container *m_container;

  transformed_renderable_items *m_transformed_items;

public:
  items_move_handler (svg_items_container *container, overlay_renderer *overlay_renderer, items_selection *selection, svg_document *document);
  ~items_move_handler ();

  void start_move (QPointF start_pos);
  void move (QPointF cur_pos);
  void end_move ();
  void add_item (const string & name);

private:
  QTransform current_transform () const;
  void apply_transform ();
};

#endif // ITEMS_MOVE_HANDLER_H
