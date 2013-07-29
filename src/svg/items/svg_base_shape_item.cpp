#include "svg/items/svg_base_shape_item.h"

#include "renderer/renderer_paint_server.h"

#include "svg/attributes/svg_attribute_stroke_linecap.h"
#include "svg/attributes/svg_attribute_stroke_linejoin.h"
#include "svg/attributes/svg_attribute_stroke_miterlimit.h"
#include "svg/attributes/svg_attribute_stroke_width.h"
#include "svg/attributes/svg_attributes_length_type_list.h"
#include "svg/attributes/svg_attributes_number.h"
#include "svg/attributes/svg_attributes_fill_stroke.h"
#include "svg/attributes/svg_attributes_enum.h"
#include "svg/attributes/svg_attribute_transform.h"

#include "svg/svg_document.h"

#include "renderer/renderer_base_shape_item.h"
#include "renderer/renderer_overlay_path.h"

#include <memory>

svg_base_shape_item::svg_base_shape_item (svg_document *document)
   : svg_base_graphics_item (document)
{
}

svg_base_shape_item::~svg_base_shape_item ()
{
}

void svg_base_shape_item::set_item_style (renderer_base_shape_item *item) const 
{
  std::unique_ptr<renderer_paint_server> fill (get_computed_attribute<svg_attribute_fill> ()->create_paint_server ());
  std::unique_ptr<renderer_paint_server> stroke (get_computed_attribute<svg_attribute_stroke> ()->create_paint_server ());
  const svg_attribute_stroke_width *stroke_width = get_computed_attribute<svg_attribute_stroke_width> ();
  const svg_attribute_stroke_linecap *stroke_linecap = get_computed_attribute<svg_attribute_stroke_linecap> ();
  const svg_attribute_stroke_linejoin *stroke_linejoin = get_computed_attribute<svg_attribute_stroke_linejoin> ();
  const svg_attribute_stroke_miterlimit *stroke_miterlimit = get_computed_attribute<svg_attribute_stroke_miterlimit> ();
  const svg_attribute_opacity *opacity = get_computed_attribute<svg_attribute_opacity> ();
  const svg_attribute_stroke_opacity *stroke_opacity = get_computed_attribute<svg_attribute_stroke_opacity> ();
  const svg_attribute_fill_opacity *fill_opacity = get_computed_attribute<svg_attribute_fill_opacity> ();
  const svg_attribute_stroke_dash_array *dash_array = get_computed_attribute<svg_attribute_stroke_dash_array> ();
  const svg_attribute_stroke_dash_offset *dash_offset = get_computed_attribute<svg_attribute_stroke_dash_offset> ();

  fill->set_opacity (fill_opacity->value () * opacity->computed_opacity ());
  stroke->set_opacity (stroke_opacity->value () * opacity->computed_opacity ());

  item->set_stroke_linecap (stroke_linecap->get_stroke_linecap ());
  item->set_stroke_linejoin (stroke_linejoin->get_stroke_linejoin ());
  item->set_stroke_miterlimit (stroke_miterlimit->get_stroke_miterlimit ());
  item->set_dash_array (dash_array->value (), dash_offset->value ());
  // For now display does the same stuff as visibility, though they little differ by specification
  item->set_stroke_width (stroke_width->get_stroke_width ());
  item->set_fill_server (fill.get ());
  item->set_stroke_server (stroke.get ());
  item->set_bounding_box (m_bbox);
}

QPainterPath svg_base_shape_item::get_path_for_clipping () const
{
  QPainterPath path = get_path ();
  const svg_attribute_clip_rule *clip_rule = get_computed_attribute<svg_attribute_clip_rule> ();
  const svg_attribute_transform *transform = get_computed_attribute<svg_attribute_transform> ();
  path = transform->computed_transform ().map (path);
  path.setFillRule (clip_rule->value () == fill_rule::EVEN_ODD ? Qt::OddEvenFill : Qt::WindingFill);
  return path;
}

renderer_graphics_item *svg_base_shape_item::create_renderer_graphics_item () const 
{
  renderer_base_shape_item *render_item = new renderer_base_shape_item (name ().toStdString ());

  QPainterPath path = get_path ();
  const svg_attribute_fill_rule *fill_rule = get_computed_attribute<svg_attribute_fill_rule> ();
  path.setFillRule (fill_rule->value () == fill_rule::EVEN_ODD ? Qt::OddEvenFill : Qt::WindingFill);
  set_item_style (render_item);
  /// must be last
  render_item->set_painter_path (path);
  return render_item;
}

QPainterPath svg_base_shape_item::get_boundaries () const 
{
  const svg_attribute_fill_rule *fill_rule = get_computed_attribute<svg_attribute_fill_rule> ();
  QPainterPath path = get_path ();
  path.setFillRule (fill_rule->value () == fill_rule::EVEN_ODD ? Qt::OddEvenFill : Qt::WindingFill);
  get_stroke (path);
  return path;
}

abstract_renderer_item *svg_base_shape_item::create_outline_renderer () const
{
  QPainterPath path = get_path ();
  path = full_transform ().map (path);
  renderer_overlay_path *overlay_item = new renderer_overlay_path (document ()->create_overlay_name ().toStdString ());
  overlay_item->set_painter_path (path);
  return overlay_item;
}

bool svg_base_shape_item::get_stroke (QPainterPath &dst) const
{
  const svg_attribute_stroke *stroke = get_computed_attribute<svg_attribute_stroke> ();
  if (!stroke->need_to_render ())
    return false;

  const svg_attribute_stroke_width *stroke_width = get_computed_attribute<svg_attribute_stroke_width> ();
  const svg_attribute_stroke_linecap *stroke_linecap = get_computed_attribute<svg_attribute_stroke_linecap> ();
  const svg_attribute_stroke_linejoin *stroke_linejoin = get_computed_attribute<svg_attribute_stroke_linejoin> ();
  const svg_attribute_stroke_miterlimit *stroke_miterlimit = get_computed_attribute<svg_attribute_stroke_miterlimit> ();

    QPainterPath path = get_path ();

  if (path.isEmpty ())
    {
      dst = QPainterPath ();
      double stroke_width_value = stroke_width->get_stroke_width ();
      if (stroke_linecap->get_stroke_linecap () == Qt::PenCapStyle::RoundCap)
        dst.addEllipse (QPointF (path.elementAt (0)), stroke_width_value, stroke_width_value);
      else if (stroke_linecap->get_stroke_linecap () == Qt::PenCapStyle::RoundCap)
        dst.addRect (path.elementAt (0).x - stroke_width_value * 0.5,
                     path.elementAt (0).y - stroke_width_value * 0.5,
                     stroke_width_value,
                     stroke_width_value
                    );
      return true;
    }

  QPainterPathStroker stroker;
  stroker.setCapStyle (stroke_linecap->get_stroke_linecap ());
  stroker.setJoinStyle (stroke_linejoin->get_stroke_linejoin ());
  stroker.setMiterLimit (stroke_miterlimit->get_stroke_miterlimit ());
  stroker.setWidth (stroke_width->get_stroke_width ());
  dst = stroker.createStroke (path);
  return true;
}
