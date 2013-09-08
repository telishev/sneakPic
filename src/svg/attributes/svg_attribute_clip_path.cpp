#include "svg/attributes/svg_attribute_clip_path.h"

#include "common/common_utils.h"

#include "svg/items/svg_item_type.h"
#include "svg/items/abstract_svg_item.h"
#include "svg/items/svg_item_clip_path.h"

#include "svg/items/svg_items_container.h"

svg_attribute_clip_path::svg_attribute_clip_path ()
{

}

svg_attribute_clip_path::~svg_attribute_clip_path ()
{

}

bool svg_attribute_clip_path::read (const char *char_data, bool /*from_css*/)
{
  QString data (char_data);
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
  CHECK (m_iri.write (url));
  data = QString ("url(%1)").arg (url);
  return true;
}

const svg_item_clip_path *svg_attribute_clip_path::clip_path (const svg_items_container *container) const
{
  if (is_empty ())
    return nullptr;

  const abstract_svg_item *path = container->get_item (m_iri.get_fragment_name ());
  if (!path || path->type () != svg_item_type::CLIP_PATH)
    return nullptr;

  return static_cast<const svg_item_clip_path *> (path);
}
