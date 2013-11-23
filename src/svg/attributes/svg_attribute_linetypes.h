#ifndef SVG_ATTRIBUTE_LINETYPES_H
#define SVG_ATTRIBUTE_LINETYPES_H

#include "svg/attributes/abstract_attribute.h"

class svg_path;

class svg_attribute_linetypes : public abstract_attribute
{
  SVG_ATTRIBUTE

  std::vector<bool> m_is_line_segment;
public:
  svg_attribute_linetypes ();
  virtual ~svg_attribute_linetypes ();

  virtual bool read (const char *data, bool from_css = false) override;
  virtual bool write (QString &data, bool to_css = false) const override;

  bool is_line_segment (int segment) const { return m_is_line_segment[segment]; }
  void set_is_line_segment (int segment, bool is_line) { m_is_line_segment[segment] = is_line; }

  void create_from_path (const svg_path *path, bool dont_create_if_exists = false);
};
#endif // SVG_ATTRIBUTE_LINETYPES_H
