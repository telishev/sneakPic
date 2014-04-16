#include "item_paint_style.h"




item_paint_style::item_paint_style ()
{

}

item_paint_style::~item_paint_style ()
{

}

void item_paint_style::load (QSettings * /*settings*/)
{

}

void item_paint_style::save (QSettings * /*settings*/)
{

}

void item_paint_style::create_from_item (abstract_svg_item *item)
{
  m_fill.create_from_item (item, true);
  m_stroke.create_from_item (item, false);
  m_stroke_cfg.create_from_item (item);
}

void item_paint_style::apply_to_item (abstract_svg_item *item) const
{
  m_fill.apply_to_item (item, true);
  m_stroke.apply_to_item (item, false);
  m_stroke_cfg.apply_to_item (item);
}

void item_paint_style::create_from_selection (items_selection *selection)
{
  m_fill.create_from_selection (selection, true);
  m_stroke.create_from_selection (selection, false);
  m_stroke_cfg.create_from_selection (selection);
}

void item_paint_style::apply_to_selection (items_selection *selection) const
{
  m_fill.apply_to_selection (selection, true);
  m_stroke.apply_to_selection (selection, false);
  m_stroke_cfg.apply_to_selection (selection);
}
