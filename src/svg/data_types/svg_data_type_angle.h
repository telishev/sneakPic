#ifndef SVG_DATA_TYPE_ANGLE_H
#define SVG_DATA_TYPE_ANGLE_H

#include <QString>

class abstract_svg_item;

enum class svg_angle_units
{
  NO_UNITS,
  DEGREE,
  GRAD,
  RAD,

  INVALID,
};

const char *enum_to_string (svg_angle_units id);
inline int enum_values_count (svg_angle_units) { return (int)svg_angle_units::INVALID; }

class svg_data_type_angle
{
  double m_value;
  svg_angle_units m_units;
public:
  svg_data_type_angle ();
  ~svg_data_type_angle ();

  bool read (const char *data, bool is_css);
  bool read_and_shift (const char *&data, bool is_css); // shifts data pointer position (for reading lists, etc. )
  QString write (bool is_css) const ;

  double value () const;
  svg_angle_units units_type () const { return m_units; }

  void set_value (double val, svg_angle_units units = svg_angle_units::NO_UNITS);

private:
  double get_units_mult () const;
};

#endif // SVG_DATA_TYPE_ANGLE_H
