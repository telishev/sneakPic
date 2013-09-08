#ifndef SVG_ATTRIBUTES_FILL_STROKE_H
#define SVG_ATTRIBUTES_FILL_STROKE_H

#include <QColor>

#include "svg/attributes/abstract_attribute.h"
#include "svg/data_types/svg_data_type_iri.h"

class abstract_svg_item;
class svg_items_container;
class renderer_paint_server;

class svg_paint_server : public abstract_attribute
{

protected:
  enum class paint_server_type
  {
    NONE,
    CURRENT_COLOR,
    COLOR,
    IRI,
  };

  svg_data_type_iri m_iri;
  QColor m_color;
  paint_server_type m_server_type;

public:
  svg_paint_server ();
  ~svg_paint_server ();

  virtual bool read (const char *data, bool from_css = false) override;
  virtual bool write (QString &data, bool to_css = false) const override;

  renderer_paint_server *create_paint_server (const svg_items_container *container) const;

  bool need_to_render (const svg_items_container *container) const;

  virtual abstract_svg_item *get_item () const = 0;
};


class svg_attribute_fill : public svg_paint_server
{
  SVG_ATTRIBUTE
public:
  svg_attribute_fill ()
  {
    m_color = Qt::black;
    m_server_type = svg_paint_server::paint_server_type::COLOR;
  }
  virtual abstract_svg_item *get_item () const override { return item (); } 
};

class svg_attribute_stroke : public svg_paint_server
{
  SVG_ATTRIBUTE
public:
  svg_attribute_stroke () {}
  virtual abstract_svg_item *get_item () const override { return item (); } 
};



#endif // SVG_ATTRIBUTES_FILL_STROKE_H

