#ifndef ITEM_HELPERS_H
#define ITEM_HELPERS_H

class svg_document;
class abstract_svg_item;

class item_helpers
{
  svg_document *m_document;
public:
  item_helpers (svg_document *document);

  abstract_svg_item *get_selectable_item_or_group (abstract_svg_item  *item) const;
  bool get_item_is_layer (abstract_svg_item  *item) const;
};

#endif // ITEM_HELPERS_H
