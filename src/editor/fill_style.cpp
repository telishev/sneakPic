#include "gui/settings.h"

#include "common/memory_deallocation.h"

#include "editor/fill_style.h"
#include "editor/items_selection.h"
#include "gui/connection.h"
#include "svg/attributes/svg_attributes_fill_stroke.h"
#include "svg/items/abstract_svg_item.h"
#include "svg/attributes/svg_attributes_number.h"

fill_style::fill_style ()
{
  m_internal_color = new QColor ();
  m_color = 0;
  m_selection = 0;
}

void fill_style::init (settings_t *settings_arg)
{
  m_color = settings_arg->fill_color ();
}

void fill_style::update_color_from_selection ()
{
  if (!m_selection)
    {
      return;
    }

  if (m_selection->count () == 0 || m_selection->count () > 1)
    {
      m_color = 0;
      return;
    }

  auto fill = (*(m_selection->begin ()))->get_computed_attribute <svg_attribute_fill> ();
  auto fill_opacity = (*(m_selection->begin ()))->get_computed_attribute <svg_attribute_fill_opacity> ();
  if (fill == 0)
    {
      m_color = 0;
      return;
    }
  paint_server_type fill_type = fill->server_type ();
  switch (fill_type)
    {
    case paint_server_type::COLOR:
      *m_internal_color = fill->color ();
      if (fill_opacity != 0)
        m_internal_color->setAlphaF (fill_opacity->value ());
      m_color = m_internal_color;
      break;
    case paint_server_type::NONE:
    case paint_server_type::IRI:
    case paint_server_type::CURRENT_COLOR:
      break;
    }
}

void fill_style::set_selection_color ()
{
  if (!m_selection || m_selection->count () == 0 || m_selection->count () > 1)
    return;

  auto fill = (*(m_selection->begin ()))->get_attribute_for_change <svg_attribute_fill> ();
  auto fill_opacity = (*(m_selection->begin ()))->get_attribute_for_change <svg_attribute_fill_opacity> ();

  fill->set_to_color (*m_color);
  fill_opacity->set_value (m_color->alphaF ());
  paint_server_type fill_type = fill->server_type ();
}

void fill_style::init (items_selection *selection)
{
  m_selection = selection;
  update_color_from_selection ();
  // In this case should update each time selection is updated
}

fill_style::~fill_style ()
{
  FREE (m_internal_color);
}

void fill_style::apply_color_to_selection ()
{
  // In this case we're updating color without calling apply changes.
  set_selection_color ();
}
