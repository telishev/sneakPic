#ifndef SVG_ATTRIBUTE_PATH_DATA_H
#define SVG_ATTRIBUTE_PATH_DATA_H

#include "svg/attributes/abstract_attribute.h"

#include <vector>

class svg_subpath;
class point_2d;

class svg_attribute_path_data : public abstract_attribute
{
  SVG_ATTRIBUTE

  std::vector<svg_subpath *> m_subpaths;
public:
  svg_attribute_path_data (abstract_svg_item *item);
  virtual ~svg_attribute_path_data ();

  virtual bool read (const QString &data, bool from_css = false) override;
  virtual bool write (QString &data, bool to_css = false) const override;

  const std::vector<svg_subpath *> &subpath () const { return m_subpaths; }

private:
  bool read_data (const char *data);

  bool is_upper (char v) const;
  unsigned char to_command (char v) const;
  void clear ();
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

