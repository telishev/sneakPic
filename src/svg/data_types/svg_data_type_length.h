#ifndef SVG_DATA_TYPE_LENGTH_H
#define SVG_DATA_TYPE_LENGTH_H

#include <QString>

class abstract_svg_item;

enum class svg_length_units
{
  NO_UNITS,
  EM,
  EX,
  PX,
  IN,
  CM,
  MM,
  PT,
  PC,
  PERCENT,

  INVALID,
};

enum class units_orientation
{
  X,
  Y,

  OTHER,
};

const char *enum_to_string (svg_length_units id);
inline int enum_values_count (svg_length_units) { return (int)svg_length_units::INVALID; }

class svg_data_type_length
{
  double m_value;
  svg_length_units m_units;
public:
  svg_data_type_length ();
  ~svg_data_type_length ();

  bool read (const char *data, bool is_css);
  QString write (bool is_css) const ;

  double value (abstract_svg_item *current_item, units_orientation orientation) const;

  void set_value (double val, svg_length_units units = svg_length_units::NO_UNITS);

private:
  double get_units_mult (abstract_svg_item *current_item, units_orientation orientation) const;
};

#endif // SVG_DATA_TYPE_LENGTH_H
