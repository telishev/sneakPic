#include "svg/attributes/svg_attribute_clip_path.h"

#include "common/common_utils.h"

#include "svg/items/svg_item_type.h"
#include "svg/items/abstract_svg_item.h"
#include "svg/items/svg_item_clip_path.h"

svg_attribute_clip_path::svg_attribute_clip_path (abstract_svg_item *item)
  : abstract_attribute (item), m_iri (item)
{

}

svg_attribute_clip_path::~svg_attribute_clip_path ()
{

}

bool svg_attribute_clip_path::read (const QString &data, bool /*from_css*/)
{
  if (!data.startsWith ("url("))
    return false;

  int begin = data.indexOf ('(');
  int end = data.indexOf (')');
  if (end == -1)
    return false;

  QString url = data.mid (begin + 1, end - begin - 1);
  if (!m_iri.read (url))
    return false;

  return true;
}

bool svg_attribute_clip_path::write (QString &data, bool /*to_css*/) const 
{
  QString url;
  CHECK (m_iri.write (data));
  data = QString ("url(%1)").arg (url);
  return true;
}

const svg_item_clip_path *svg_attribute_clip_path::clip_path () const
{
  const abstract_svg_item *path = m_iri.href ();
  if (!path || path->type () != svg_item_type::CLIP_PATH)
    return nullptr;

  return static_cast<const svg_item_clip_path *> (path);
}
