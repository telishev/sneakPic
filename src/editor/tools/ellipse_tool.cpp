#include "ellipse_tool.h"

#include "common/memory_deallocation.h"

#include "editor/items_selection.h"

#include "gui/actions_applier.h"
#include "gui/settings.h"

#include "renderer/overlay_renderer.h"
#include "renderer/svg_painter.h"
#include "renderer/renderer_overlay_path.h"

#include "svg/attributes/svg_attributes_length_type.h"
#include "svg/items/svg_item_ellipse.h"
#include "svg/svg_document.h"
#include "editor/operations/add_item_operation.h"
#include "editor/style_controller.h"
#include "editor/item_paint_style.h"

ellipse_tool::ellipse_tool( svg_painter *painter )
  : abstract_tool (painter)
{
  m_items_container = m_painter->item_container ();
  m_actions_applier->add_drag_shortcut (mouse_drag_shortcut_t::CREATE_ELLIPSE, this,
    &ellipse_tool::start_ellipse_positioning, &ellipse_tool::continue_ellipse_positioning, &ellipse_tool::end_ellipse_positioning);

  m_renderer_item = new renderer_overlay_path ();
  m_overlay->add_item (m_renderer_item, overlay_layer_type::TEMP);
}

ellipse_tool::~ellipse_tool ()
{
  FREE (m_renderer_item);
}

bool ellipse_tool::start_ellipse_positioning (const QPoint &pos)
{
  if (!can_add_items ())
    return false;

  m_initial_pos = m_painter->get_local_pos (QPointF (pos));
  return true;
}

QRectF ellipse_tool::get_rect (const QPointF &pos)
{
  return QRectF (qMin (m_initial_pos.x (), pos.x ()), qMin (m_initial_pos.y (), pos.y ()),
                 qAbs (m_initial_pos.x () - pos.x ()), qAbs (m_initial_pos.y () - pos.y ()));
}

void ellipse_tool::update_preview (QPointF &current_pos)
{
  QPainterPath path;
  path.addRect (get_rect (current_pos));
  path.addEllipse (get_rect (current_pos));
  m_renderer_item->set_painter_path (path);
  m_renderer_item->set_visible (true);
}

bool ellipse_tool::continue_ellipse_positioning (const QPoint &pos)
{
  QPointF current_pos = m_painter->get_local_pos (QPointF (pos));
  update_preview (current_pos);
  m_painter->redraw ();
  return true;
}

bool ellipse_tool::end_ellipse_positioning (const QPoint &pos)
{
  QPointF final_pos = m_painter->get_local_pos (QPointF (pos));
  hide_preview ();
  insert_item (final_pos);
  return true;
}

void ellipse_tool::hide_preview ()
{
  m_renderer_item->set_visible (false);
}

void ellipse_tool::insert_item (const QPointF &pos )
{
  auto *ellipse_item = m_painter->document ()->create_new_svg_item<svg_item_ellipse> ();
  if (ellipse_item == nullptr)
    return;

  QRectF rect = get_rect (pos);
  ellipse_item->get_attribute_for_change<svg_attribute_rx> ()->set_value (rect.width () / 2);
  ellipse_item->get_attribute_for_change<svg_attribute_ry> ()->set_value (rect.height () / 2);
  ellipse_item->get_attribute_for_change<svg_attribute_cx> ()->set_value ((rect.left () + rect.right ()) / 2);
  ellipse_item->get_attribute_for_change<svg_attribute_cy> ()->set_value ((rect.top () + rect.bottom ()) / 2);

  add_item_operation (m_painter).apply (ellipse_item);
  m_painter->document ()->apply_changes ("Create Ellipse");
}

