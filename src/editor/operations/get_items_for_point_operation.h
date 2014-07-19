#ifndef GET_ITEMS_FOR_POINT_OPERATION_H
#define GET_ITEMS_FOR_POINT_OPERATION_H

class svg_document;
class abstract_svg_item;
class QPointF;
enum class selection_type_t;


class get_items_for_point_operation
{
  svg_document *m_document;
  selection_type_t m_type;
public:
  get_items_for_point_operation (svg_document *document, selection_type_t type);
  ~get_items_for_point_operation ();

  bool is_point_inside (QPointF point, const abstract_svg_item *item) const;
  std::vector<const abstract_svg_item *> get_items_for_point (QPointF point) const;
  const abstract_svg_item *get_prev_item_for_point (const abstract_svg_item *current, QPointF point) const;
};


#endif // GET_ITEMS_FOR_POINT_OPERATION_H


