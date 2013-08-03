#include "svg/attributes/svg_attributes_marker_usage.h"

#include <QPainterPath>
#include <QString>

#include "common/math_defs.h"
#include "common/string_utils.h"

#include "svg/items/abstract_svg_item.h"
#include "svg/items/svg_item_marker.h"
#include "svg/items/svg_item_type.h"

bool svg_base_attribute_marker_usage::read (const char *data, bool /*from_css*/ /*= false*/)
{
  if (strcmp (data, "none") == 0)
    m_element = nullptr;

  m_element = new svg_data_type_iri (item ());
  m_element->read (QString (data));
  if (m_element->get_type () != iri_type::document_fragment)
    FREE (m_element);
  return true;
}

bool svg_base_attribute_marker_usage::write (QString &data, bool /*from_css*/ /*= false*/) const 
{
  if (m_element)
    m_element->write (data);
  else
    data = "none";
  return true;
}

std::vector<abstract_renderer_item *> svg_base_attribute_marker_usage::configure_markers_on_path_drawing (QPainterPath path, QTransform transform, double stroke_width) const
{
  std::vector<abstract_renderer_item *> renderer_items;
  if (!m_element)
    return renderer_items;

  abstract_svg_item *item = m_element->get_fragment ();
  if (item->type () != svg_item_type::MARKER)
    return renderer_items;

  svg_item_marker *marker_item = static_cast <svg_item_marker *> (item);
  QPointF first_vector, second_vector;
  int focused_index;

  for (int i = 0; i < path.elementCount (); i++)
    {
      focused_index = i;
      if (i == 0)
        {
          if (!m_start)
            continue;

        }
      else if (i == path.elementCount () - 1 || (i == path.elementCount () - 3 && path.elementAt (focused_index).type == QPainterPath::CurveToElement))
        {
          if (!m_end)
            continue;
        }
      else
        {
          if (!m_mid)
            continue;

        }

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
        renderer_items.push_back (item);
    }
  return renderer_items;
}
