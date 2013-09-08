#ifndef SVG_ATTRIBUTE_POINTS_H
#define SVG_ATTRIBUTE_POINTS_H

#include "svg/attributes/abstract_attribute.h"

#include "svg/data_types/svg_data_type_length.h"

#include <QList>
#include <QPointF>

class svg_attribute_points : public abstract_attribute
{
  SVG_ATTRIBUTE
private:
  QList <QPointF> m_point_list;
public:
  svg_attribute_points ();
  virtual ~svg_attribute_points ();

  QList <QPointF> value () const { return m_point_list; };
  virtual bool read (const char *data, bool from_css = false) override;
  virtual bool write (QString &data, bool to_css = false) const override;
};
#endif // SVG_ATTRIBUTE_POINTS_H
