#include "svg/attributes/svg_attributes_string.h"
#include "svg/attributes/svg_attribute_font_size.h"
#include "svg/attributes/svg_attributes_length_type.h"

#include "svg/items/svg_item_text.h"

#include <QFont>
#include <QPainterPath>
#include <QTransform>

svg_item_text::svg_item_text (svg_document *document)
  : svg_base_shape_item (document)
{
}

svg_item_text::~svg_item_text ()
{
}

bool svg_item_text::check_item ()
{
  return true;
}

bool svg_item_text::read_item (const QString &data)
{
  m_text = data;
  m_text = m_text.trimmed ();
  return true;
}

QPainterPath svg_item_text::get_path () const 
{
  QPainterPath path;
  const svg_attribute_font_family *font_family = get_computed_attribute <svg_attribute_font_family> ();
  const svg_attribute_font_size *font_size = get_computed_attribute <svg_attribute_font_size> ();
  const svg_attribute_x *x = get_computed_attribute <svg_attribute_x> ();
  const svg_attribute_y *y = get_computed_attribute <svg_attribute_y> ();
  QFont font (font_family->value ());
  QTransform transform;
  double scale = 100.0 / font_size->value ();
  transform.scale (scale, scale);
  font.setPixelSize(font_size->value () * scale);
  path.addText (transform.map (QPointF (x->value (), y->value ())), font, m_text);
  path = transform.inverted ().map (path);
  return path;
}