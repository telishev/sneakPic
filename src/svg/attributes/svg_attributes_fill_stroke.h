#ifndef SVG_ATTRIBUTES_FILL_STROKE_H
#define SVG_ATTRIBUTES_FILL_STROKE_H

#include <QColor>

#include "svg/attributes/abstract_attribute.h"
#include "svg/data_types/data_type_iri.h"

class renderer_paint_server;

class svg_paint_server : public abstract_attribute
{

protected:
  enum class paint_server_type
  {
    NONE,
    COLOR,
    IRI,
  };

  data_type_iri m_iri;
  QColor m_color;
  paint_server_type m_server_type;

public:
  svg_paint_server (abstract_svg_item *item);
  ~svg_paint_server ();

  virtual bool read (const QString &data, bool from_css = false) override;
  virtual bool write (QString &data, bool to_css = false) const override;

  renderer_paint_server *create_paint_server () const;
};


class svg_attribute_fill : public svg_paint_server
{
  SVG_ATTRIBUTE
public:
  svg_attribute_fill (abstract_svg_item *item) : svg_paint_server (item)
  {
    m_color = Qt::black;
    m_server_type = svg_paint_server::paint_server_type::COLOR;
  }
};

class svg_attribute_stroke : public svg_paint_server
{
  SVG_ATTRIBUTE
public:
  svg_attribute_stroke (abstract_svg_item *item) : svg_paint_server (item) {}
};



#endif // SVG_ATTRIBUTES_FILL_STROKE_H

