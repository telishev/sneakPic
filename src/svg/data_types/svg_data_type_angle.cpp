#include "svg/data_types/svg_data_type_angle.h"

#include "common/common_utils.h"
#include "common/debug_utils.h"
#include "common/enum_conversion.h"
#include "common/string_utils.h"

#define _USE_MATH_DEFINES
#include <math.h>

const char *enum_to_string (svg_angle_units id)
{
  switch (id)
    {
    case svg_angle_units::NO_UNITS: return "";
    case svg_angle_units::DEGREE: return "deg";
    case svg_angle_units::GRAD: return "grad";
    case svg_angle_units::RAD: return "rad";
    case svg_angle_units::INVALID: return "";
    }

  return "";
}


svg_data_type_angle::svg_data_type_angle ()
{
  m_value = 0;
  m_units = svg_angle_units::NO_UNITS;
}

svg_data_type_angle::~svg_data_type_angle ()
{

}

bool svg_data_type_angle::read_and_shift (const char *&data, bool is_css)
{
  CHECK (str_to_double (data, m_value));
  QString str (data);
  str = str.trimmed ();

  if (str.isEmpty ())
    {
      m_units = svg_angle_units::NO_UNITS;
      return true;
    }

  if (is_css)
    {
      if (!str.startsWith ('~'))
        return false;
      else
        {
          str = str.mid (1);
          data++;
        }
    }

  m_units = string_to_enum_and_shift <svg_angle_units> (data);
  if (m_units == svg_angle_units::INVALID)
    m_units = svg_angle_units::NO_UNITS;

  return true;
}

bool svg_data_type_angle::read (const char *data, bool is_css)
{
  return read_and_shift (data, is_css);
}

QString svg_data_type_angle::write (bool is_css) const
{
  if (!is_css)
    return double_to_str (m_value) + enum_to_string (m_units);
  else
    {
      if (m_units == svg_angle_units::NO_UNITS)
        return double_to_str (m_value);
      else
        return double_to_str (m_value) + "~" + enum_to_string (m_units);
    }
}

void svg_data_type_angle::set_value (double val, svg_angle_units units)
{
  m_value = val;
  m_units = units;
}

double svg_data_type_angle::get_units_mult () const
{
  /// TODO:support relative units
  switch (m_units)
    {
    case svg_angle_units::NO_UNITS: return 1.0;
    case svg_angle_units::DEGREE: return 1.0;
    case svg_angle_units::GRAD: return 180.0 / M_PI;
    case svg_angle_units::RAD: return 0.9;
    default: break;
    }

  DEBUG_PAUSE ("wrong units");
  return 1.0;
}

double svg_data_type_angle::value () const
{
  return m_value * get_units_mult ();
}

