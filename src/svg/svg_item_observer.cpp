#include "svg/svg_item_observer.h"

#include "common/common_utils.h"




svg_item_observer::svg_item_observer ()
{

}

svg_item_observer::~svg_item_observer ()
{

}

void svg_item_observer::child_added (const std::string &sender, const std::string &child_name, int insert_pos)
{
  FIX_UNUSED (sender, child_name, insert_pos);
}

void svg_item_observer::child_removed (const std::string &sender, const std::string &child_name, int pos)
{
  FIX_UNUSED (sender, child_name, pos);
}

void svg_item_observer::child_moved (const std::string &sender, const std::string &child_name, int old_pos, int new_pos)
{
  FIX_UNUSED (sender, child_name, old_pos, new_pos);
}

void svg_item_observer::attribute_change_start (const std::string &sender, const abstract_attribute *computed_attribute)
{
  FIX_UNUSED (sender, computed_attribute);
}

void svg_item_observer::attribute_change_end (const std::string &sender, const abstract_attribute *computed_attribute)
{
  FIX_UNUSED (sender, computed_attribute);
}

void svg_item_observer::item_removed (const std::string &sender)
{
  FIX_UNUSED (sender);
}
