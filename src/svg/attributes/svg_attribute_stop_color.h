#ifndef SVG_ATTRIBUTE_STOP_COLOR_H
#define SVG_ATTRIBUTE_STOP_COLOR_H

#include "svg/attributes/abstract_attribute.h"

#include <QColor>

class svg_attribute_stop_color : public abstract_attribute
{
  SVG_ATTRIBUTE

  QColor m_stop_color;
public:
  svg_attribute_stop_color (abstract_svg_item *item) : abstract_attribute (item) { }
  virtual ~svg_attribute_stop_color () {}

  virtual bool read (const QString &data, bool /*from_css*/ = false) override
  {
    m_stop_color = QColor (data);
    return true;
  }

  virtual bool write (QString &data, bool /*to_css*/ = false) const override
  {
    data = data = QString ("#%1%2%3").arg (m_stop_color.red ()).arg (m_stop_color.green ()).arg (m_stop_color.blue ());
    return true;
  }

  QColor stop_color () const { return m_stop_color; }

};


#endif // SVG_ATTRIBUTE_STOP_COLOR_H
