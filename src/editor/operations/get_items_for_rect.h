#ifndef GET_ITEMS_FOR_RECT_H
#define GET_ITEMS_FOR_RECT_H

class abstract_svg_item;
class svg_document;

class get_items_for_rect_operation
{
  svg_document *m_document;
public:
  get_items_for_rect_operation (svg_document *doc);
  ~get_items_for_rect_operation ();

  std::vector<const abstract_svg_item *> apply (QRectF rect);
};

#endif // GET_ITEMS_FOR_RECT_H
