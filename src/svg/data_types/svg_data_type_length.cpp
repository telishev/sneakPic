#include "svg/data_types/svg_data_type_length.h"

#include "common/debug_utils.h"
#include "common/common_utils.h"
#include "common/string_utils.h"
#include "common/enum_conversion.h"

#include "svg/items/abstract_svg_item.h"
#include "svg/items/svg_item_type.h"

#include "svg/attributes/svg_attribute_font_size.h"
#include "svg/attributes/svg_attributes_length_type.h"
#include "svg/attributes/svg_attribute_viewbox.h"

#include <qmath.h>

const char *enum_to_string (svg_length_units id)
{
  switch (id)
    {
    case svg_length_units::NO_UNITS: return "";
    case svg_length_units::PX: return "px";
    case svg_length_units::IN: return "in";
    case svg_length_units::CM: return "cm";
    case svg_length_units::MM: return "mm";
    case svg_length_units::PT: return "pt";
    case svg_length_units::PC: return "pc";
    case svg_length_units::PERCENT: return "%";
    case svg_length_units::EM: return "em";
    case svg_length_units::EX: return "ex";
    case svg_length_units::INVALID: return "";
    }

  return "";
}


svg_data_type_length::svg_data_type_length ()
{
  m_value = 0;
  m_units = svg_length_units::NO_UNITS;
}

svg_data_type_length::~svg_data_type_length ()
{

}

bool svg_data_type_length::read (const char *data, bool is_css)
{
  CHECK (str_to_double (data, m_value));
  QString str (data);
  str = str.trimmed ();

  if (str.isEmpty ())
    {
      m_units = svg_length_units::NO_UNITS;
      return true;
    }

  if (is_css)
    {
      if (!str.startsWith ('~'))
        return false;
      else
        str = str.mid (1);
    }

  m_units = string_to_enum<svg_length_units> (str.toUtf8 ().constData ());
  if (m_units == svg_length_units::INVALID)
    {
      m_units = svg_length_units::NO_UNITS;
      return false;
    }

  return true;
}

QString svg_data_type_length::write (bool is_css) const
{
  if (!is_css)
    return double_to_str (m_value) + enum_to_string (m_units);
  else
    {
      if (m_units == svg_length_units::NO_UNITS)
        return double_to_str (m_value);
      else
        return double_to_str (m_value) + "~" + enum_to_string (m_units);
    }
}

void svg_data_type_length::set_value (double val, svg_length_units units)
{
  m_value = val;
  m_units = units;
}

static double get_required_viewport_size (abstract_svg_item *current_item, units_orientation orientation)
{
  for (; current_item; current_item = current_item->parent ())
    {
      if (current_item->type () == svg_item_type::SVG)
        {
          double width = current_item->get_computed_attribute <svg_attribute_width> ()->value ();
          double height = current_item->get_computed_attribute <svg_attribute_height> ()->value ();
          // Probably later viewbox checking and getting actual width, height should be moved to svg (viewport) item
          const svg_attribute_view_box *viewbox = current_item->get_computed_attribute <svg_attribute_view_box> ();
          if (viewbox)
            {
              width = viewbox->get_width ();
              height = viewbox->get_height ();
            }

          switch (orientation)
            {
            case units_orientation::X:
              return width;
            case units_orientation::Y:
              return height;
            case units_orientation::OTHER:
              return qSqrt ((width * width + height * height) * 0.5);
            }
        }
    }
  return 100.0;
}

double svg_data_type_length::get_units_mult (abstract_svg_item *current_item, units_orientation orientation) const
{
  /// TODO:support relative units
  switch (m_units)
    {
    case svg_length_units::NO_UNITS: return 1.0;
    case svg_length_units::PX: return 1.0;
    case svg_length_units::IN: return 90.0;
    case svg_length_units::CM: return 35.43307;
    case svg_length_units::MM: return 3.543307;
    case svg_length_units::PT: return 1.25;
    case svg_length_units::PC: return 15.0;
    case svg_length_units::EM: return (current_item ? current_item->get_computed_attribute <svg_attribute_font_size> ()->value () : 1.0);
    case svg_length_units::EX: return 1.0;
    case svg_length_units::PERCENT: return get_required_viewport_size (current_item, orientation) * 0.01; // dividing by 100 because it's percent
    default: break;
    }

  DEBUG_PAUSE ("wrong units");
  return 1.0;
}

double svg_data_type_length::value (abstract_svg_item *current_item, units_orientation orientation) const
{
  return m_value * get_units_mult (current_item, orientation);
}

