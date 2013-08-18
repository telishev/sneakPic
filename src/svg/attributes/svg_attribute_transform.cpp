#include "svg/attributes/svg_attribute_transform.h"

#include <QStringList>

#include "common/string_utils.h"
#include "common/common_utils.h"

#include "svg/items/abstract_svg_item.h"


svg_attribute_transform::svg_attribute_transform (svg_document *document)
  : abstract_attribute (document)
{

}

svg_attribute_transform::~svg_attribute_transform ()
{

}

bool svg_attribute_transform::read (const char *data, bool /*from_css*/)
{
  return m_transform.read (data);
}

bool svg_attribute_transform::write (QString &data, bool /*to_css*/) const 
{
  return m_transform.write (data);
}

QTransform svg_attribute_transform::computed_transform () const
{
  return m_additional_transform * m_transform.transform ();
}

void svg_attribute_transform::set_additional_transform (const QTransform &additional_transform)
{
  m_additional_transform = additional_transform;
}

void svg_attribute_transform::set_transform (const QTransform &transform)
{
  m_transform.set_transform (transform);
  m_additional_transform = QTransform ();
}
