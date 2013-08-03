#include "svg/attributes/svg_attributes_enum.h"
#include "svg/attributes/svg_attributes_string.h"
#include "svg/attributes/svg_attribute_font_size.h"
#include "svg/attributes/svg_attributes_length_type.h"
#include "svg/attributes/svg_attributes_length_type_list.h"

#include "svg/items/svg_item_text.h"

#include <QFont>
#include <QFontMetrics>
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
  const svg_attribute_dx *dx = get_computed_attribute <svg_attribute_dx> ();
  const svg_attribute_dy *dy = get_computed_attribute <svg_attribute_dy> ();
  const svg_attribute_text_anchor *attr_text_anchor = get_computed_attribute <svg_attribute_text_anchor> ();
  double x_value = x->value ();
  double y_value = y->value ();
  QList<double> dx_value, dy_value;
  if (dx)
    {
      dx->fill_values (dx_value);
      if (dx_value.size () == 1)
        x_value += dx_value[0]; // Other cases for now - unsupported
    }

  if (dy)
    {
      dy->fill_values (dy_value);
      if (dy_value.size () == 1) // Other cases for now - unsupported
        y_value += dy_value[0];
    }

  QFont font;
  QTransform transform;
  double scale = 100.0 / font_size->value ();
  transform.scale (scale, scale);
  font.setFamily (font_family->value ());
  font.setPixelSize(font_size->value () * scale);
  font.setWeight (QFont::Light);
  QFontMetrics metrics (font);
  QPointF transformed_point = transform.map (QPointF (x_value, y_value));
  x_value = transformed_point.x ();
  y_value = transformed_point.y ();
  switch (attr_text_anchor->value ())
    {
      case text_anchor::MIDDLE: x_value -= metrics.width (m_text) * 0.5; break;
      case text_anchor::END: x_value -= metrics.width (m_text); break;
      default: break; // do nothing
    };
  path.addText (x_value, y_value, font, m_text);
  path = transform.inverted ().map (path);
  return path;
}
