#include "duplicate_item_operation.h"
#include "svg/items/abstract_svg_item.h"
#include "svg/svg_document.h"
#include "svg/items/svg_item_factory.h"
#include "svg/attributes/svg_attribute_type.h"
#include "add_item_operation.h"
#include "svg/undo/undo_handler.h"

duplicate_item_operation::duplicate_item_operation (svg_painter *painter /*= nullptr*/, bool duplicate_clones /*= false*/)
{
  m_painter=  painter;
  m_duplicate_clones = duplicate_clones;
}

abstract_svg_item * duplicate_item_operation::apply (abstract_svg_item *item)
{
  if (!m_duplicate_clones && item->is_cloned ())
    return nullptr;

  svg_document *document = item->document ();
  abstract_svg_item *copy = document->item_factory ()->create_item (item->type_name (), item->namespace_uri (), item->namespace_name ());
  if (!copy)
    return nullptr;

  document->get_undo_handler ()->add_item (copy);
  copy->update_name ();

  for (int i = 0; i < (int)svg_attribute_type::COUNT; i++)
    {
      svg_attribute_type type = (svg_attribute_type)i;
      if (type == svg_attribute_type::ID)
        continue;

      const abstract_attribute *attribute = item->get_computed_attribute_by_type (type);
      if (!attribute)
        continue;

      copy->add_attribute (attribute->clone ());
    }

  if (m_painter)
    {
      add_item_operation add_op (m_painter, false);
      add_op.set_apply_style (false);
      add_op.apply (copy);
    }

  for (int i = 0; i < item->children_count (); i++)
    {
      auto copy_child = duplicate_item_operation (nullptr, m_duplicate_clones).apply (item->child (i));
      if (copy_child)
        copy->push_back (copy_child);
    }

  copy->process_after_read ();

  return copy;
}
