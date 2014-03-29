#ifndef SVG_ATTRIBUTE_STOP_COLOR_H
#define SVG_ATTRIBUTE_STOP_COLOR_H

#include "svg/attributes/abstract_attribute.h"

#include "common/string_utils.h"

#include <QColor>



class svg_attribute_stop_color : public abstract_attribute
{
  SVG_ATTRIBUTE

  QColor m_stop_color;
public:
  svg_attribute_stop_color () { }
  virtual ~svg_attribute_stop_color () {}

  virtual bool read (const char *data, bool /*from_css*/ = false) override
  {
    m_stop_color = QColor (data);
    return true;
  }

  virtual bool write (QString &data, bool /*to_css*/ = false) const override
  {
    data = color_to_string (m_stop_color);
    return true;
  }

  QColor stop_color () const { return m_stop_color; }
  void set_stop_color (QColor color) { m_stop_color = color; }

};


#endif // SVG_ATTRIBUTE_STOP_COLOR_H
