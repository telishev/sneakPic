#include "svg/data_types/svg_data_type_length.h"

#include "common/debug_utils.h"
#include "common/common_utils.h"
#include "common/string_utils.h"
#include "common/enum_conversion.h"


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

double svg_data_type_length::get_units_mult () const
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
    case svg_length_units::EM: return 1.0;
    case svg_length_units::EX: return 1.0;
    case svg_length_units::PERCENT: return 1.0;
    default: break;
    }

  DEBUG_PAUSE ("wrong units");
  return 1.0;
}

double svg_data_type_length::value () const
{
  return m_value * get_units_mult ();
}

