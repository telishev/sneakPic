#include "editor/operations/apply_style_operation.h"

#include "svg/items/abstract_svg_item.h"

#include "svg/attributes/svg_attribute_stroke_width.h"
#include "svg/attributes/svg_attribute_stroke_linejoin.h"
#include "svg/attributes/svg_attribute_stroke_miterlimit.h"
#include "svg/attributes/svg_attribute_stroke_linecap.h"
#include "svg/attributes/svg_attributes_fill_stroke.h"
#include "svg/attributes/svg_attributes_number.h"

#include "gui/settings.h"



apply_style_operation::apply_style_operation (const settings_t *settings)
{
  m_settings = settings;
}

apply_style_operation::~apply_style_operation ()
{

}

void apply_style_operation::apply (abstract_svg_item *svg_item)
{
  svg_item->get_attribute_for_change<svg_attribute_stroke_width> ()->set_value (m_settings->stroke_width ());
  svg_item->get_attribute_for_change<svg_attribute_stroke_linejoin> ()->set_value (m_settings->stroke_linejoin ());
  svg_item->get_attribute_for_change<svg_attribute_stroke_miterlimit> ()->set_value (m_settings->stroke_miterlimit ());
  svg_item->get_attribute_for_change<svg_attribute_stroke_linecap> ()->set_value (m_settings->stroke_linecap ());

  {
    auto fill = svg_item->get_attribute_for_change<svg_attribute_fill> ();
    fill->set_to_color (*m_settings->fill_color ());
    svg_item->get_attribute_for_change<svg_attribute_fill_opacity> ()->set_value (m_settings->fill_color ()->alphaF ());
  }

  {
    auto stroke = svg_item->get_attribute_for_change<svg_attribute_stroke> ();
    stroke->set_to_color (*m_settings->stroke_color ());
    svg_item->get_attribute_for_change<svg_attribute_stroke_opacity> ()->set_value (m_settings->stroke_color ()->alphaF ());
  }
}
