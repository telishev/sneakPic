#include "svg/items/svg_item_stop.h"

#include <QColor>

#include "svg/attributes/svg_attribute_stop_color.h"
#include "svg/attributes/svg_attributes_number.h"
#include "svg/attributes/svg_attribute_offset.h"
#include "svg/items/svg_item_type.h"


svg_item_stop::svg_item_stop (svg_document *document)
  : abstract_svg_item (document)
{

}

svg_item_stop::~svg_item_stop ()
{

}

double svg_item_stop::offset () const
{
  double my_offset = get_computed_attribute<svg_attribute_offset> ()->offset ();
  
  /// offset must be greater than previous offsets
  for (int sibling_index = child_index () - 1; sibling_index >= 0;  sibling_index--)
    {
      const abstract_svg_item *sibling = parent ()->child (sibling_index);
      if (sibling->type () != svg_item_type::STOP)
        continue;

      const svg_item_stop *prev_stop = static_cast<const svg_item_stop *> (sibling);
      double prev_offset = prev_stop->offset ();
      my_offset = qMax (my_offset, prev_offset);
      break;
    }

  return my_offset;
}

QColor svg_item_stop::color () const
{
  QColor color = get_computed_attribute<svg_attribute_stop_color> ()->stop_color ();
  color.setAlphaF (get_computed_attribute<svg_attribute_stop_opacity> ()->value ());
  return color;
}

bool svg_item_stop::check_item ()
{
  return true;
}


