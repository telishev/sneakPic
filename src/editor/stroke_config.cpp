#include "stroke_config.h"
#include "svg/items/abstract_svg_item.h"
#include "svg/attributes/svg_attribute_stroke_width.h"
#include "svg/attributes/svg_attribute_stroke_linejoin.h"
#include "svg/attributes/svg_attribute_stroke_miterlimit.h"
#include "svg/attributes/svg_attribute_stroke_linecap.h"
#include "svg/attributes/svg_attributes_fill_stroke.h"
#include "svg/attributes/svg_attributes_number.h"
#include "items_selection.h"




stroke_config::stroke_config ()
{
  m_width = 1.0;
  m_linejoin = Qt::SvgMiterJoin;
  m_miterlimit = 4.0;
  m_linecap = Qt::FlatCap;
}

stroke_config::~stroke_config ()
{

}

void stroke_config::create_from_item (abstract_svg_item *item)
{
  m_width = item->get_computed_attribute <svg_attribute_stroke_width> ()->value ();
  m_linejoin = item->get_computed_attribute <svg_attribute_stroke_linejoin> ()->value ();
  m_miterlimit = item->get_computed_attribute <svg_attribute_stroke_miterlimit> ()->value ();
  m_linecap = item->get_computed_attribute <svg_attribute_stroke_linecap> ()->value ();
}

void stroke_config::apply_to_item (abstract_svg_item *item) const
{
  item->get_attribute_for_change <svg_attribute_stroke_width> ()->set_value (m_width);
  item->get_attribute_for_change <svg_attribute_stroke_linejoin> ()->set_value (m_linejoin);
  item->get_attribute_for_change <svg_attribute_stroke_miterlimit> ()->set_value (m_miterlimit);
  item->get_attribute_for_change <svg_attribute_stroke_linecap> ()->set_value (m_linecap);
}

void stroke_config::create_from_selection (items_selection *selection)
{
  abstract_svg_item *item = selection->single_item ();
  if (!item)
    return;

  create_from_item (item);
}

void stroke_config::apply_to_selection (items_selection *selection) const
{
  for (abstract_svg_item *item : *selection)
    apply_to_item (item);
}
