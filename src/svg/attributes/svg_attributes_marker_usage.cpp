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
  if (strcmp (data, "none") == 0)
    return true;

  m_iri.read (QString (data));
  if (m_iri.get_type () != iri_type::document_fragment)
    m_iri.set_to_invalid ();

  update_observers ();
  return true;
}

bool svg_base_attribute_marker_usage::write (QString &data, bool /*from_css*/ /*= false*/) const 
{
  if (m_iri.is_valid ())
    m_iri.write (data);
  else
    data = "none";
  return true;
}

string svg_base_attribute_marker_usage::fragment_name () const
{
  return m_iri.get_fragment_name ();
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
