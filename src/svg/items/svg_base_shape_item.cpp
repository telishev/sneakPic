#include "svg/items/svg_base_shape_item.h"

#include <memory>

#include "common/math_defs.h"

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
#include "svg/attributes/svg_attributes_marker_usage.h"

#include "svg/svg_document.h"

#include "svg/items/svg_items_container.h"
#include "svg/items/svg_item_marker.h"
#include "svg/items/svg_item_type.h"

#include "renderer/renderer_base_shape_item.h"
#include "renderer/renderer_overlay_path.h"


svg_base_shape_item::svg_base_shape_item (svg_document *document)
   : svg_base_graphics_item (document)
{
}

svg_base_shape_item::~svg_base_shape_item ()
{
}

void svg_base_shape_item::set_item_style (renderer_base_shape_item *item) const
{
  unique_ptr<renderer_paint_server> fill (get_computed_attribute<svg_attribute_fill> ()->create_paint_server (document ()->item_container ()));
  unique_ptr<renderer_paint_server> stroke (get_computed_attribute<svg_attribute_stroke> ()->create_paint_server (document ()->item_container ()));
  auto stroke_width = get_computed_attribute<svg_attribute_stroke_width> ();
  auto stroke_linecap = get_computed_attribute<svg_attribute_stroke_linecap> ();
  auto stroke_linejoin = get_computed_attribute<svg_attribute_stroke_linejoin> ();
  auto stroke_miterlimit = get_computed_attribute<svg_attribute_stroke_miterlimit> ();
  auto opacity = get_computed_attribute<svg_attribute_opacity> ();
  auto stroke_opacity = get_computed_attribute<svg_attribute_stroke_opacity> ();
  auto fill_opacity = get_computed_attribute<svg_attribute_fill_opacity> ();
  auto dash_array = get_computed_attribute<svg_attribute_stroke_dash_array> ();
  auto dash_offset = get_computed_attribute<svg_attribute_stroke_dash_offset> ();

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

  configure_markers (item);
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
  renderer_base_shape_item *render_item = new renderer_base_shape_item (name ());

  QPainterPath path = get_path ();
  const svg_attribute_fill_rule *attr_fill_rule = get_computed_attribute<svg_attribute_fill_rule> ();
  path.setFillRule (attr_fill_rule->value () == fill_rule::EVEN_ODD ? Qt::OddEvenFill : Qt::WindingFill);
  render_item->set_painter_path (path);
  set_item_style (render_item);
  return render_item;
}

QPainterPath svg_base_shape_item::get_boundaries () const
{
  const svg_attribute_fill_rule *attr_fill_rule = get_computed_attribute<svg_attribute_fill_rule> ();
  QPainterPath path = get_path ();
  path.setFillRule (attr_fill_rule->value () == fill_rule::EVEN_ODD ? Qt::OddEvenFill : Qt::WindingFill);
  get_stroke (path);
  return path;
}

renderable_item *svg_base_shape_item::create_outline_renderer () const
{
  QPainterPath path = get_path ();
  path = full_transform ().map (path);
  renderer_overlay_path *overlay_item = new renderer_overlay_path;
  overlay_item->set_painter_path (path);
  return overlay_item;
}

bool svg_base_shape_item::get_stroke (QPainterPath &dst) const
{
  const svg_attribute_stroke *stroke = get_computed_attribute<svg_attribute_stroke> ();
  if (!stroke->need_to_render (document ()->item_container ()))
    return false;

  const svg_attribute_stroke_width *stroke_width = get_computed_attribute<svg_attribute_stroke_width> ();
  const svg_attribute_stroke_linecap *stroke_linecap = get_computed_attribute<svg_attribute_stroke_linecap> ();
  const svg_attribute_stroke_linejoin *stroke_linejoin = get_computed_attribute<svg_attribute_stroke_linejoin> ();
  const svg_attribute_stroke_miterlimit *stroke_miterlimit = get_computed_attribute<svg_attribute_stroke_miterlimit> ();

  QPainterPath path = get_path ();

  if (path.isEmpty () && !path.currentPosition ().isNull ())
    {
      dst = QPainterPath ();
      double stroke_width_value = stroke_width->get_stroke_width ();
      if (stroke_linecap->get_stroke_linecap () == Qt::PenCapStyle::RoundCap)
        dst.addEllipse (QPointF (path.elementAt (0)), stroke_width_value, stroke_width_value);
      else if (stroke_linecap->get_stroke_linecap () == Qt::PenCapStyle::SquareCap)
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

  auto fill = get_computed_attribute<svg_attribute_fill> ();
  if (fill->need_to_render (document ()->item_container ()))
    dst = stroker.createStroke (path).united (dst);
  else
    dst = stroker.createStroke (path);
  return true;
}

void svg_base_shape_item::configure_markers_on_path_drawing (renderer_base_shape_item *base_item,
  const svg_base_attribute_marker_usage *marker, const QPainterPath &path, const QTransform &transform, double stroke_width) const
{
  abstract_svg_item *item = document ()->item_container ()->get_item (marker->fragment_name ());
  if (!item || item->type () != svg_item_type::MARKER)
    return;

  svg_item_marker *marker_item = static_cast <svg_item_marker *> (item);
  QPointF first_vector, second_vector;
  int focused_index;

  for (int i = 0; i < path.elementCount (); i++)
    {
      focused_index = i;
      if (!marker->is_point_applicable (i, path))
        continue;

      if (path.elementAt (focused_index).type == QPainterPath::CurveToDataElement)
        continue;

      if (path.elementAt (focused_index).type == QPainterPath::CurveToElement)
        focused_index += 2;

      if (path.elementAt (focused_index).isMoveTo () || i == 0)
        first_vector = QPointF (0.0, 0.0);
      else
        first_vector = path.elementAt (focused_index) - path.elementAt (focused_index - 1);

      if (focused_index == path.elementCount () - 1)
        second_vector = QPointF (0.0, 0.0);
      else 
        second_vector = path.elementAt (focused_index + 1) - path.elementAt (focused_index);
      
      if (are_equal (first_vector.x (), second_vector.x ()) && are_equal (first_vector.y (), second_vector.y ()))
        continue;

      abstract_renderer_item *item = marker_item->create_renderer_item_for_marker (path.elementAt (focused_index), first_vector + second_vector, transform, stroke_width);
      if (item != nullptr)
        base_item->add_marker (item);
    }
}

void svg_base_shape_item::configure_markers (renderer_base_shape_item *item) const
{
  QTransform transform = full_transform ();
  auto marker_start = get_computed_attribute<svg_attribute_marker_start> ();
  auto marker_mid = get_computed_attribute<svg_attribute_marker_mid> ();
  auto marker_end = get_computed_attribute<svg_attribute_marker_end> ();
  auto marker = get_computed_attribute<svg_attribute_marker> ();
  auto stroke_width = get_computed_attribute<svg_attribute_stroke_width> ();

  configure_markers_on_path_drawing (item, marker_start, item->painter_path (), transform, stroke_width->get_stroke_width ());
  configure_markers_on_path_drawing (item, marker_mid, item->painter_path (), transform, stroke_width->get_stroke_width ());
  configure_markers_on_path_drawing (item, marker_end, item->painter_path (), transform, stroke_width->get_stroke_width ());
  configure_markers_on_path_drawing (item, marker, item->painter_path (), transform, stroke_width->get_stroke_width ());
}