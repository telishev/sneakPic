#ifndef ITEM_HELPERS_H
#define ITEM_HELPERS_H

class svg_document;
class abstract_svg_item;

enum class selection_type_t
{
  SELECT_ITEMS_AND_GROUPS,
  SELECT_ONLY_ITEMS,
};

class item_helpers
{
  svg_document *m_document;
public:
  item_helpers (svg_document *document);

  const abstract_svg_item *get_selectable_item_or_group (const abstract_svg_item *item, selection_type_t type) const;
  bool get_item_is_layer (const abstract_svg_item  *item) const;
  bool is_item_selectable (const abstract_svg_item *item, selection_type_t type) const;
};

#endif // ITEM_HELPERS_H
