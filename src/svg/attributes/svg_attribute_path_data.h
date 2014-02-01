#ifndef SVG_ATTRIBUTE_PATH_DATA_H
#define SVG_ATTRIBUTE_PATH_DATA_H

#include "svg/attributes/abstract_attribute.h"

#include <vector>

class QPainterPath;
class svg_path_geom;
class path_builder;

class svg_attribute_path_data : public abstract_attribute
{
  SVG_ATTRIBUTE

  svg_path_geom *m_path;
public:
  svg_attribute_path_data ();
  virtual ~svg_attribute_path_data ();

  virtual bool read (const char *data, bool from_css = false) override;
  virtual bool write (QString &data, bool to_css = false) const override;

  QPainterPath create_painter_path () const;
  svg_path_geom *path () const { return m_path; }

private:
  bool is_upper (char v) const;
  unsigned char to_command (char v) const;

  bool read_move (const char *&data, path_builder &builder, bool relative);
  bool read_end_subpath (const char *&data, path_builder &builder, bool relative);
  bool read_line (const char *&data, path_builder &builder, bool relative);
  bool read_h_line (const char *&data, path_builder &builder, bool relative);
  bool read_v_line (const char *&data, path_builder &builder, bool relative);
  bool read_curve (const char *&data, path_builder &builder, bool relative);
  bool read_curve_short (const char *&data, path_builder &builder, bool relative);
  bool read_quadratic (const char *&data, path_builder &builder, bool relative);
  bool read_quadratic_short (const char *&data, path_builder &builder, bool relative);
  bool read_arc (const char *&data, path_builder &builder, bool relative);
};


#endif // SVG_ATTRIBUTE_PATH_DATA_H

