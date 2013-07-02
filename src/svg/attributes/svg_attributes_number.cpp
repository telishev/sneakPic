#include "svg/attributes/svg_attributes_number.h"

#include <QString>

#include "svg/items/abstract_svg_item.h"


bool svg_base_attribute_number::read (const QString &data, bool /*from_css*/ /*= false*/)
{
  bool ok;
  m_value = data.toDouble (&ok);
  return ok;
}

bool svg_base_attribute_number::write (QString &data, bool /*from_css*/ /*= false*/) const 
{
  data = QString::number (m_value);
  return true;
}

double svg_attribute_opacity::computed_opacity () const
{
  /// TODO: this is totally wrong, opacity is not multiplied  by its parent value,
  /// but instead the whole group is rendered first, and then is blended onto surface
  const abstract_svg_item *parent = item () ? item ()->parent () : nullptr;
  if (!parent)
    return m_value;

  const svg_attribute_opacity *base_opacity = parent->get_computed_attribute <svg_attribute_opacity> ();
  return m_value * base_opacity->computed_opacity ();
}
