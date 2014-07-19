#include "get_items_for_point_operation.h"

#include <QPainterPath>

#include "svg/items/svg_graphics_item.h"
#include "svg/items/svg_base_graphics_item.h"
#include "common/range_algorithm.h"
#include "editor/item_helpers.h"
#include "svg/svg_document.h"
#include "svg/items/items_comparison.h"

get_items_for_point_operation::get_items_for_point_operation (svg_document *document, selection_type_t type)
{
  m_document = document;
  m_type = type;
}

get_items_for_point_operation::~get_items_for_point_operation ()
{

}

bool get_items_for_point_operation::is_point_inside (QPointF point, const abstract_svg_item *item) const
{
  if (!item)
    return false;

  const svg_graphics_item *graph_item = item->to_graphics_item ();
  if (!graph_item)
    return false;

  if (!graph_item->bbox ().contains (point))
    return false;

  const svg_base_graphics_item *base_graph_item = graph_item->to_base_graphics_item ();
  if (base_graph_item)
    return base_graph_item->get_transformed_boundaries ().contains (point);
  else
    return range::any_of (*item, [&] (const abstract_svg_item *child) { return is_point_inside (point, child); });
}

class items_for_point_gatherer
{
  QPointF m_point;
  selection_type_t m_type;
  item_helpers m_helper;
  std::vector<const abstract_svg_item *> m_result;
  get_items_for_point_operation m_get_items_op;
public:
  items_for_point_gatherer (QPointF point, selection_type_t type, svg_document *document)
    : m_point (point), m_type (type), m_helper (document), m_get_items_op (document, type) {}

  std::vector<const abstract_svg_item *> & get_result () { return m_result; }

  void get_items_for_point_recursive (const abstract_svg_item *item)
  {
    const svg_graphics_item *graph_item = item->to_graphics_item ();
    if (!graph_item)
      return;

    if (!graph_item->bbox ().contains (m_point))
      return;

    if (m_helper.is_item_selectable (item, m_type))
      {
        if (m_get_items_op.is_point_inside (m_point, item))
          m_result.push_back (item);
      }
    else
      for (const abstract_svg_item *child : *item)
        get_items_for_point_recursive (child);
  }
};

std::vector<const abstract_svg_item *> get_items_for_point_operation::get_items_for_point (QPointF point) const
{
  items_for_point_gatherer gatherer (point, m_type, m_document);
  gatherer.get_items_for_point_recursive (m_document->root ());
  return std::move (gatherer.get_result ());
}

const abstract_svg_item *get_items_for_point_operation::get_prev_item_for_point (const abstract_svg_item *current, QPointF point) const
{
  std::vector<const abstract_svg_item *> items = get_items_for_point (point);
  if (items.empty ())
    return nullptr;

  if (!current)
    return items.back ();

  auto it = std::lower_bound (items.begin (), items.end (), current, items_comparison_z_order ());
  if (it == items.begin ())
    return items.back ();

  return *(it - 1);
}
