#include "editor/style_controller.h"

#include "editor/fill_style.h"
#include "editor/stroke_style.h"
#include "editor/items_selection.h"
#include "editor/style_container.h"

#include "renderer/svg_painter.h"

#include "svg/svg_document.h"
#include "gui/connection.h"

style_controller::style_controller (settings_t *settings_arg)
{
  for (int i = 0; i < (int) selected_style::COUNT; i++)
    m_containers[i] = new style_container ();

  m_current_style = selected_style::EDITOR_STYLE;
  m_containers[(int) selected_style::EDITOR_STYLE]->init (settings_arg);
  m_painter = 0;
}

void style_controller::update_stroke_width (double value)
{
  active_container ()->get_stroke_style ()->update_width (value);
  apply_changes ();
};

void style_controller::update_stroke_miterlimit (double value)
{
  active_container ()->get_stroke_style ()->update_miterlimit (value);
  apply_changes ();
};

void style_controller::update_linejoin (Qt::PenJoinStyle value)
{
  active_container ()->get_stroke_style ()->update_linejoin (value);
  apply_changes ();
};

void style_controller::update_linecap (Qt::PenCapStyle value)
{
  active_container ()->get_stroke_style ()->update_linecap (value);
  apply_changes ();
};

style_container *style_controller::active_container ()
{
  return m_containers[(int) m_current_style];
}

const style_container *style_controller::active_container () const
{
  return m_containers[(int) m_current_style];
}

void style_controller::set_painter (svg_painter *painter)
{
  m_containers[(int) selected_style::SELECTED_STYLE]->init (painter->selection ());
  CONNECT (painter->selection (), &items_selection::selection_changed, this, &style_controller::selection_or_items_changed);

  CONNECT (painter, &svg_painter::color_picked, this, &style_controller::update_from_color_picker);
  m_painter = painter;
}

void style_controller::switch_to (selected_style current_style_arg)
{
  m_current_style = current_style_arg;
  selection_or_items_changed ();
}

void style_controller::update_fill_color_momentarily ()
{
  active_container ()->get_fill_style ()->apply_color_to_selection ();
  if (m_painter)
  {
    m_painter->document ()->redraw ();
  }
}

void style_controller::update_stroke_color_momentarily ()
{
  active_container ()->get_stroke_style ()->apply_color_to_selection ();
  if (m_painter)
  {
    m_painter->document ()->redraw ();
  }
}

void style_controller::apply_changes ()
{
  if (m_painter)
    m_painter->document ()->apply_changes ("Change Style");
}

void style_controller::selection_or_items_changed ()
{
  active_container ()->get_fill_style ()->update_from_selection ();
  active_container ()->get_stroke_style ()->update_from_selection ();
  emit target_items_changed ();
}

void style_controller::update_from_color_picker (const QColor &color)
{
  active_container ()->get_fill_style ()->set_color (color);
  active_container ()->get_fill_style ()->apply_color_to_selection ();
  apply_changes ();
  emit target_items_changed ();
}

double style_controller::stroke_width () const
{
  return active_container ()->get_stroke_style ()->stroke_width ();
}

double style_controller::stroke_miterlimit () const
{
  return active_container ()->get_stroke_style ()->stroke_miterlimit ();
}

Qt::PenJoinStyle style_controller::stroke_linejoin () const
{
  return active_container ()->get_stroke_style ()->stroke_linejoin ();
}

Qt::PenCapStyle style_controller::stroke_linecap () const
{
  return active_container ()->get_stroke_style ()->stroke_linecap ();
}
