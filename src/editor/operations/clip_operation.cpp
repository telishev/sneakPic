#include "clip_operation.h"
#include "common/debug_utils.h"
#include "svg/items/svg_base_shape_item.h"
#include "operation_get_defs_item.h"
#include "svg/items/svg_item_clip_path.h"
#include "svg/svg_document.h"
#include "svg/attributes/svg_attribute_clip_path.h"
#include "svg/attributes/svg_attributes_enum.h"




void clip_operation::apply (abstract_svg_item *clip_item, const std::vector<abstract_svg_item *> &items)
{
  if (!items.size ())
    return;

  svg_document *document = clip_item->document ();
  auto clip_path = document->create_new_svg_item<svg_item_clip_path> ();
  operation_get_defs_item (document).apply ()->push_back (clip_path);
  clip_item->parent ()->make_orphan (clip_item);
  clip_path->adopt_orphan (clip_item);

  auto fill_rule = clip_item->get_computed_attribute<svg_attribute_fill_rule> ();
  clip_item->get_attribute_for_change<svg_attribute_clip_rule> ()->set_value (fill_rule->value ());

  for (auto && item : items)
    {
      item->get_attribute_for_change<svg_attribute_clip_path> ()->set_clip_path (clip_path);
    }
}
