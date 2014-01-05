#ifndef SVG_ATTRIBUTE_STROKE_MITERLIMIT_H
#define SVG_ATTRIBUTE_STROKE_MITERLIMIT_H

#include "svg/attributes/abstract_attribute.h"

#include <QString>

class svg_attribute_stroke_miterlimit : public abstract_attribute
{
  SVG_ATTRIBUTE

  double m_miterlimit;
public:
  svg_attribute_stroke_miterlimit ();
  virtual ~svg_attribute_stroke_miterlimit ();

  double get_stroke_miterlimit () const { return m_miterlimit; }

  virtual bool read (const char *data, bool from_css = false) override;
  virtual bool write (QString &data, bool to_css = false) const override;
  void set_value (double value) { m_miterlimit = value; }
  double value () const { return m_miterlimit; }
};
#endif // SVG_ATTRIBUTE_STROKE_MITERLIMIT_H
