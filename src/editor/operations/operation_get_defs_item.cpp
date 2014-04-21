#include "operation_get_defs_item.h"

#include "svg/items/abstract_svg_item.h"
#include "svg/svg_document.h"
#include "svg/items/svg_item_type.h"
#include "svg/items/svg_item_defs.h"


operation_get_defs_item::operation_get_defs_item (svg_document *document)
{
  m_document = document;
}

abstract_svg_item * operation_get_defs_item::apply ()
{
  for (auto &&item : *m_document->root ())
    {
      if (item->type () == svg_item_type::DEFS)
        return item;
    }

  auto defs = m_document->create_new_svg_item<svg_item_defs> ();
  m_document->root ()->push_back (defs);
  return defs;
}
