#include "rectangle_tool.h"

#include "common/memory_deallocation.h"

#include "editor/items_selection.h"

#include "gui/actions_applier.h"
#include "gui/settings.h"

#include "renderer/overlay_renderer.h"
#include "renderer/renderer_paint_server.h"
#include "renderer/renderer_base_shape_item.h"
#include "renderer/svg_painter.h"

#include "svg/attributes/svg_attributes_length_type.h"
#include "svg/attributes/svg_attributes_fill_stroke.h"
#include "svg/attributes/svg_attributes_number.h"
#include "svg/attributes/svg_attribute_stroke_linejoin.h"
#include "svg/attributes/svg_attribute_stroke_width.h"
#include "svg/items/svg_items_container.h"
#include "svg/items/svg_item_rect.h"
#include "svg/svg_document.h"

rectangle_tool::rectangle_tool( svg_painter *painter )
  : abstract_tool (painter)
{
  m_items_container = m_painter->item_container ();
  m_actions_applier->add_drag_shortcut (mouse_drag_shortcut_enum::CREATE_RECTANGLE, this,
    &rectangle_tool::start_rectangle_positioning, &rectangle_tool::continue_rectangle_positioning, &rectangle_tool::end_rectangle_positioning);

  m_renderer_item = new renderer_base_shape_item ("");
  m_renderer_item->set_ignore_bbox (true);
  m_overlay->add_item (m_renderer_item, overlay_layer_type::TEMP);
}

rectangle_tool::~rectangle_tool ()
{
  FREE (m_renderer_item);
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

  renderer_painter_server_color fill_server (*(m_painter->settings ()->fill_color ()));
  fill_server.set_opacity (m_painter->settings ()->fill_color ()->alphaF ());
  m_renderer_item->set_fill_server (&fill_server);

  renderer_painter_server_color stroke_server (*(m_painter->settings ()->stroke_color ()));
  stroke_server.set_opacity (m_painter->settings ()->stroke_color ()->alphaF ());
  m_renderer_item->set_stroke_server (&stroke_server);

  m_renderer_item->set_stroke_width (m_painter->settings ()->stroke_width ());
  m_renderer_item->set_stroke_linejoin (m_painter->settings ()->stroke_linejoin ());
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

void rectangle_tool::insert_item (const QPointF &pos )
{
  auto *rect_item = m_painter->document ()->create_new_svg_item<svg_item_rect> ();

  QRectF rect = get_rect (pos);
  rect_item->get_attribute_for_change<svg_attribute_x> ()->set_value (rect.left ());
  rect_item->get_attribute_for_change<svg_attribute_y> ()->set_value (rect.top ());
  rect_item->get_attribute_for_change<svg_attribute_width> ()->set_value (rect.width ());
  rect_item->get_attribute_for_change<svg_attribute_height> ()->set_value (rect.height ());

  rect_item->get_attribute_for_change<svg_attribute_stroke_width> ()->set_value (m_painter->settings ()->stroke_width ());
  rect_item->get_attribute_for_change<svg_attribute_stroke_linejoin> ()->set_value (m_painter->settings ()->stroke_linejoin ());

  {
    auto fill = rect_item->get_attribute_for_change<svg_attribute_fill> ();
    fill->set_to_color (*m_painter->settings ()->fill_color ());
    rect_item->get_attribute_for_change<svg_attribute_fill_opacity> ()->set_value (m_painter->settings ()->fill_color ()->alphaF ());
  }

  {
    auto stroke = rect_item->get_attribute_for_change<svg_attribute_stroke> ();
    stroke->set_to_color (*m_painter->settings ()->stroke_color ());
    rect_item->get_attribute_for_change<svg_attribute_stroke_opacity> ()->set_value (m_painter->settings ()->stroke_color ()->alphaF ());
  }

  rect_item->update_bbox ();
  m_painter->document ()->root ()->push_back (rect_item);

  items_selection *selection = m_painter->selection ();
  selection->clear ();
  selection->add_item (rect_item);

  m_painter->document ()->apply_changes ();
}

