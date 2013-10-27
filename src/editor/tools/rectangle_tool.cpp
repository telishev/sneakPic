#include "rectangle_tool.h"

#include "gui/mouse_shortcuts_handler.h"
#include "gui/settings.h"

#include "renderer/overlay_renderer.h"
#include "renderer/renderer_base_shape_item.h"
#include "renderer/svg_painter.h"

#include "svg/attributes/svg_attributes_length_type.h"
#include "svg/items/svg_items_container.h"
#include "svg/items/svg_item_rect.h"
#include "svg/svg_document.h"

void rectangle_tool::activate()
{
}

void rectangle_tool::deactivate()
{
}

void rectangle_tool::configure()
{
}

void rectangle_tool::draw( QPainter &painter, const QRect &rect_to_draw, const QTransform &transform)
{
  m_overlay->draw (painter, rect_to_draw, transform);
}

bool rectangle_tool::mouse_event( const mouse_event_t &m_event)
{
  return m_mouse_handler->process_mouse_event (m_event);
}

rectangle_tool::rectangle_tool( svg_painter *painter )
  :abstract_tool (painter)
{
   m_mouse_handler = new mouse_shortcuts_handler (m_painter->settings ()->shortcuts_cfg ());
   m_overlay = new overlay_renderer;
   m_items_container = m_painter->item_container ();
   ADD_SHORTCUT_DRAG (m_mouse_handler, CREATE_RECTANGLE,
     return start_rectangle_positioning (m_event.pos ()),
     return continue_rectangle_positioning (m_event.pos ()),
     return end_rectangle_positioning (m_event.pos ()));

  m_renderer_item = new renderer_base_shape_item ("");
  m_overlay->add_item (m_renderer_item, overlay_layer_type::TEMP);
}

bool rectangle_tool::start_rectangle_positioning (const QPoint &pos)
{
  m_initial_pos = m_painter->get_local_pos (QPointF (pos));
  return true;
}

QRectF rectangle_tool::get_rect (const QPointF &pos)
{
  return QRectF (qMin (m_initial_pos.x (), pos.x ()), qMin (m_initial_pos.y (), pos.y ()),
                 qAbs (m_initial_pos.x () - pos.x ()), qAbs (m_initial_pos.y () - pos.y ()));
}

void rectangle_tool::update_preview (QPointF &current_pos)
{
  QPainterPath path;
  path.addRect (get_rect (current_pos));
  m_renderer_item->set_visibility (true);
  m_renderer_item->set_painter_path (path);
  m_renderer_item->set_bounding_box (path.boundingRect ());
  m_renderer_item->update_bbox ();
}

bool rectangle_tool::continue_rectangle_positioning (const QPoint &pos)
{
  QPointF current_pos = m_painter->get_local_pos (QPointF (pos));
  update_preview (current_pos);
  m_painter->redraw ();
  return true;
}

bool rectangle_tool::end_rectangle_positioning (const QPoint &pos)
{
  QPointF final_pos = m_painter->get_local_pos (QPointF (pos));
  hide_preview ();
  insert_item (final_pos);
  return true;
}

void rectangle_tool::hide_preview ()
{
  m_renderer_item->set_visibility (false);
}

void rectangle_tool::insert_item( const QPointF &pos )
{
  svg_item_rect *rect_item = new svg_item_rect (m_painter->document ());
  QRectF rect = get_rect (pos);
  rect_item->get_attribute_for_change<svg_attribute_x> ()->set_value (rect.left ());
  rect_item->get_attribute_for_change<svg_attribute_y> ()->set_value (rect.top ());
  rect_item->get_attribute_for_change<svg_attribute_width> ()->set_value (rect.width ());
  rect_item->get_attribute_for_change<svg_attribute_height> ()->set_value (rect.height ());
  rect_item->update_bbox ();
  rect_item->check ();
  m_painter->document ()->root ()->push_back (rect_item);
  m_painter->document ()->apply_changes ();
}
