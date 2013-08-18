#ifndef SVG_ATTRIBUTE_PATH_DATA_H
#define SVG_ATTRIBUTE_PATH_DATA_H

#include "svg/attributes/abstract_attribute.h"

#include <vector>

class point_2d;
class QPainterPath;

enum class path_command
{
  MOVE,
  CLOSE_PATH,
  LINE,
  CURVE,
  QUAD,
  ARC,
};

class svg_attribute_path_data : public abstract_attribute
{
  SVG_ATTRIBUTE

  std::vector<double> m_path_data;
  std::vector<path_command> m_commands;
public:
  svg_attribute_path_data (svg_document *document);
  virtual ~svg_attribute_path_data ();

  virtual bool read (const char *data, bool from_css = false) override;
  virtual bool write (QString &data, bool to_css = false) const override;

  QPainterPath create_painter_path () const;

private:
  bool is_upper (char v) const;
  unsigned char to_command (char v) const;

  int command_data_count (path_command command) const;
  const char *command_svg_name (path_command command) const;

  bool read_move (const char *&data, point_2d &subpath_start_point, point_2d &current_point, bool relative);
  bool read_end_subpath (const char *&data, point_2d &subpath_start_point, point_2d &current_point, bool relative);
  bool read_line (const char *&data, point_2d &subpath_start_point, point_2d &current_point, bool relative);
  bool read_h_line (const char *&data, point_2d &subpath_start_point, point_2d &current_point, bool relative);
  bool read_v_line (const char *&data, point_2d &subpath_start_point, point_2d &current_point, bool relative);
  bool read_curve (const char *&data, point_2d &subpath_start_point, point_2d &current_point, bool relative);
  bool read_curve_short (const char *&data, point_2d &subpath_start_point, point_2d &current_point, bool relative);
  bool read_quadratic (const char *&data, point_2d &subpath_start_point, point_2d &current_point, bool relative);
  bool read_quadratic_short (const char *&data, point_2d &subpath_start_point, point_2d &current_point, bool relative);
  bool read_arc (const char *&data, point_2d &subpath_start_point, point_2d &current_point, bool relative);
};


#endif // SVG_ATTRIBUTE_PATH_DATA_H

