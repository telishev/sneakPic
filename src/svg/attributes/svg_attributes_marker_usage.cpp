#include "svg/attributes/svg_attributes_marker_usage.h"

#include <QPainterPath>
#include <QString>

#include "common/math_defs.h"
#include "common/memory_deallocation.h"
#include "common/string_utils.h"

#include "svg/items/svg_item_marker.h"
#include "svg/items/svg_item_type.h"
#include "svg/svg_document.h"

bool svg_base_attribute_marker_usage::read (const char *data, bool /*from_css*/ /*= false*/)
{
  FREE (m_element);
  if (strcmp (data, "none") == 0)
    {
      m_element = nullptr;
      return true;
    }
  
  m_element = new svg_data_type_iri;
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

string svg_base_attribute_marker_usage::fragment_name () const
{
  return m_element ? m_element->get_fragment_name () : string ();
}

bool svg_attribute_marker_start::is_point_applicable (int number, const QPainterPath &/*path*/) const 
{
  return number == 0;
}

bool svg_attribute_marker_mid::is_point_applicable (int number, const QPainterPath &path) const 
{
  return (number == path.elementCount () - 1 || (number == path.elementCount () - 3 && path.elementAt (number).type == QPainterPath::CurveToElement));
}

bool svg_attribute_marker_end::is_point_applicable (int number, const QPainterPath &path) const 
{
  if (number == 0)
    return false;

  if (number == path.elementCount () - 1 || (number == path.elementCount () - 3 && path.elementAt (number).type == QPainterPath::CurveToElement))
    return false;

  return true;
}
