/// \file viz/abstract_painter.cpp
/// \brief class used to draw something on canvas_widget

#include "renderer/abstract_painter.h"

#include "gui/canvas_widget_t.h"

abstract_painter::abstract_painter (canvas_widget_t *canvas_widget)
  : QObject (canvas_widget)
{
  m_canvas_widget = canvas_widget;
  set_configure_all (1);
}

abstract_painter::~abstract_painter ()
{
}

void abstract_painter::set_configure_all (int value)
{
  for (int i = 0; i < (int)configure_type::COUNT; i++)
    set_configure_needed ((configure_type)i, value);
}

void abstract_painter::update ()
{
  set_configure_needed (configure_type::REDRAW_BASE, 1);
  canvas_widget ()->update ();
}
