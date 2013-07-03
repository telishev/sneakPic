#include "svg/attributes/svg_attribute_transform.h"

#include <QStringList>

#include "common/string_utils.h"
#include "common/common_utils.h"

#include "svg/items/abstract_svg_item.h"


svg_attribute_transform::svg_attribute_transform (abstract_svg_item *item)
  : abstract_attribute (item)
{

}

svg_attribute_transform::~svg_attribute_transform ()
{

}

bool svg_attribute_transform::read (const QString &data, bool /*from_css*/)
{
  return m_transform.read (data);
}

bool svg_attribute_transform::write (QString &data, bool /*to_css*/) const 
{
  return m_transform.write (data);
}

QTransform svg_attribute_transform::computed_transform () const
{
  const abstract_svg_item *parent = item () ? item ()->parent () : nullptr;
  if (!parent)
    return m_transform.transform ();

  const svg_attribute_transform *base_transform = parent->get_computed_attribute <svg_attribute_transform> ();
  return m_additional_transform * m_transform.transform () * base_transform->computed_transform ();
}

void svg_attribute_transform::set_additional_transform (const QTransform &additional_transform)
{
  m_additional_transform = additional_transform;
}
