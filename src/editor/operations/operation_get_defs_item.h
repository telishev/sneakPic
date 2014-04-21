#ifndef OPERATION_GET_DEFS_ITEM_H
#define OPERATION_GET_DEFS_ITEM_H

class abstract_svg_item;
class svg_document;

class operation_get_defs_item
{
  svg_document *m_document;
public:
  operation_get_defs_item (svg_document *document);

  abstract_svg_item *apply ();
};

#endif // OPERATION_GET_DEFS_ITEM_H
