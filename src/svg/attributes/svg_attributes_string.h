#ifndef SVG_ATTRIBUTES_STRING_H
#define SVG_ATTRIBUTES_STRING_H

#include <QString>

#include "svg/attributes/abstract_attribute.h"

class svg_base_attribute_string : public abstract_attribute
{
protected:
  QString m_value;
public:
  svg_base_attribute_string () { m_value = 0.0; }

  QString value () const { return m_value; }
  void set_value (const char *value) { m_value = value; }
  void set_value (const QString &value) { m_value = value; }

  virtual bool read (const char *data, bool from_css = false) override;
  virtual bool write (QString &data, bool from_css = false) const override;
};


class svg_attribute_font_family : public svg_base_attribute_string { SVG_ATTRIBUTE public: svg_attribute_font_family () {};};
class svg_attribute_layer_name : public svg_base_attribute_string { SVG_ATTRIBUTE public: svg_attribute_layer_name () {};};
class svg_attribute_active_layer : public svg_base_attribute_string { SVG_ATTRIBUTE public: svg_attribute_active_layer () {};};

#endif // SVG_ATTRIBUTES_STRING_H
