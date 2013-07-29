#include "svg_graphics_item.h"

#include <QTransform>

#include "svg/attributes/svg_attribute_transform.h"


svg_graphics_item::svg_graphics_item (svg_document *document)
  : abstract_svg_item (document)
{

}

svg_graphics_item::~svg_graphics_item ()
{

}

QTransform svg_graphics_item::full_transform () const
{
  QTransform total_transform;
  for (const abstract_svg_item *cur_item = this; cur_item; cur_item = cur_item->parent ())
    {
      const svg_attribute_transform *base_transform = cur_item->get_computed_attribute <svg_attribute_transform> ();
      total_transform = total_transform * base_transform->computed_transform ();
    }

  return total_transform;
}