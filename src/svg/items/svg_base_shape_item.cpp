#include "svg/items/svg_base_shape_item.h"

#include "renderer/renderer_base_shape_item.h"
#include "renderer/renderer_paint_server.h"

#include "svg/attributes/svg_attributes_enum.h"
#include "svg/attributes/svg_attribute_style.h"
#include "svg/attributes/svg_attribute_stroke_linecap.h"
#include "svg/attributes/svg_attribute_stroke_linejoin.h"
#include "svg/attributes/svg_attribute_stroke_miterlimit.h"
#include "svg/attributes/svg_attribute_stroke_width.h"
#include "svg/attributes/svg_attribute_transform.h"
#include "svg/attributes/svg_attributes_number.h"
#include "svg/attributes/svg_attributes_fill_stroke.h"
#include "svg/attributes/svg_attribute_clip_path.h"
#include "svg/attributes/svg_attributes_enum.h"

#include "svg/items/svg_item_clip_path.h"

#include "renderer/renderer_item_path.h"

#include <memory>
#include <QPainterPath>
#include <QTransform>


svg_base_shape_item::svg_base_shape_item (svg_document *document)
   : abstract_svg_item (document)
{
  m_render_item = nullptr;
}

svg_base_shape_item::~svg_base_shape_item ()
{
  FREE (m_render_item);
}

void svg_base_shape_item::set_item_style (renderer_item_path *item)
{
  std::unique_ptr<renderer_paint_server> fill (get_computed_attribute<svg_attribute_fill> ()->create_paint_server ());
  std::unique_ptr<renderer_paint_server> stroke (get_computed_attribute<svg_attribute_stroke> ()->create_paint_server ());
  const svg_attribute_stroke_width *stroke_width = get_computed_attribute<svg_attribute_stroke_width> ();
  const svg_attribute_stroke_linecap *stroke_linecap = get_computed_attribute<svg_attribute_stroke_linecap> ();
  const svg_attribute_stroke_linejoin *stroke_linejoin = get_computed_attribute<svg_attribute_stroke_linejoin> ();
  const svg_attribute_stroke_miterlimit *stroke_miterlimit = get_computed_attribute<svg_attribute_stroke_miterlimit> ();
  const svg_attribute_transform *transform = get_computed_attribute<svg_attribute_transform> ();
  const svg_attribute_opacity *opacity = get_computed_attribute<svg_attribute_opacity> ();
  const svg_attribute_stroke_opacity *stroke_opacity = get_computed_attribute<svg_attribute_stroke_opacity> ();
  const svg_attribute_fill_opacity *fill_opacity = get_computed_attribute<svg_attribute_fill_opacity> ();
  const svg_item_clip_path *clip_path = get_computed_attribute<svg_attribute_clip_path> ()->clip_path ();
  const svg_attribute_visibility *visibility = get_computed_attribute<svg_attribute_visibility> ();

  fill->set_opacity (fill_opacity->value ());
  stroke->set_opacity (stroke_opacity->value ());

  item->set_stroke_linecap (stroke_linecap->get_stroke_linecap ());
  item->set_stroke_linejoin (stroke_linejoin->get_stroke_linejoin ());
  item->set_stroke_miterlimit (stroke_miterlimit->get_stroke_miterlimit ());
  item->set_stroke_visibility (visibility->value () == visibility::VISIBLE);
  item->set_stroke_width (stroke_width->get_stroke_width ());
  item->set_transform (transform->computed_transform ());
  item->set_opacity (opacity->computed_opacity ());
  item->set_fill_server (fill.get ());
  item->set_stroke_server (stroke.get ());
  if (clip_path)
    item->set_clip_path (clip_path->get_clip_path ());
}

QPainterPath svg_base_shape_item::get_path_for_clipping () const
{
  QPainterPath path = get_path ();
  const svg_attribute_clip_rule *clip_rule = get_computed_attribute<svg_attribute_clip_rule> ();
  path = full_transform ().map (path);
  path.setFillRule (clip_rule->value () == fill_rule::EVEN_ODD ? Qt::OddEvenFill : Qt::WindingFill);
  return path;
}

void svg_base_shape_item::update_renderer_item ()
{
  m_render_item = new renderer_item_path;

  QPainterPath path = get_path ();
  const svg_attribute_fill_rule *fill_rule = get_computed_attribute<svg_attribute_fill_rule> ();
  path.setFillRule (fill_rule->value () == fill_rule::EVEN_ODD ? Qt::OddEvenFill : Qt::WindingFill);
  set_item_style (m_render_item);
  /// must be last
  m_render_item->set_painter_path (path, full_transform ());
}

const abstract_renderer_item *svg_base_shape_item::get_renderer_item () const 
{
  return m_render_item;
}

QTransform svg_base_shape_item::full_transform () const
{
  QTransform total_transform;
  for (const abstract_svg_item *cur_item = this; cur_item; cur_item = cur_item->parent ())
    {
      const svg_attribute_transform *base_transform = cur_item->get_computed_attribute <svg_attribute_transform> ();
      total_transform = total_transform * base_transform->computed_transform ();
    }

  return total_transform;
}


