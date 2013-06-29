
#ifndef SVG_ATTRIBUTE_STROKE_H
#define SVG_ATTRIBUTE_STROKE_H

#include "svg/attributes/abstract_attribute.h"

#include <QColor>

class svg_attribute_stroke : public abstract_attribute
{
  SVG_ATTRIBUTE

  QColor m_color;
  bool m_applied;
public:
  svg_attribute_stroke (abstract_svg_item *item);
  virtual ~svg_attribute_stroke ();

  QColor get_color () const { return m_color; }
  bool is_applied () const { return m_applied; }

  virtual bool read (const QString &data, bool from_css = false) override;
  virtual bool write (QString &data, bool to_css = false) const override;
};
#endif // SVG_ATTRIBUTE_STROKE_H

