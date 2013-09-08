#ifndef SVG_ATTRIBUTE_VIEWBOX_H
#define SVG_ATTRIBUTE_VIEWBOX_H

#include "svg/attributes/abstract_attribute.h"

class QTransform;
class QRectF;

class svg_attribute_view_box : public abstract_attribute
{
  SVG_ATTRIBUTE
  double m_x, m_y, m_width, m_height;
public:
  svg_attribute_view_box ();
  virtual ~svg_attribute_view_box ();

  virtual bool read (const char *data, bool from_css = false) override;
  virtual bool write (QString &data, bool to_css = false) const override;

  QTransform get_transform (const QRectF &viewport) const;
  double get_width () const {return m_width; }
  double get_height () const {return m_height; }
};


#endif // SVG_ATTRIBUTE_VIEWBOX_H
