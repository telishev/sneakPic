#include "editor/operations/apply_style_operation.h"

#include "svg/items/abstract_svg_item.h"

#include "editor/item_paint_style.h"


apply_style_operation::apply_style_operation (const item_paint_style *style)
{
  m_style = style;
}

apply_style_operation::~apply_style_operation ()
{

}

void apply_style_operation::apply (abstract_svg_item *svg_item)
{
  m_style->fill ().apply_to_item (svg_item, true);
  m_style->stroke ().apply_to_item (svg_item, false);
  m_style->stroke_cfg ().apply_to_item (svg_item);
}
