/// \file viz/abstract_painter.cpp
/// \brief class used to draw something on glwidget

#include "renderer/abstract_painter.h"

#include "gui/gl_widget.h"

abstract_painter::abstract_painter (gl_widget *glwidget)
  : QObject (glwidget)
{
  m_glwidget = glwidget;
  set_configure_all (1);
}

abstract_painter::~abstract_painter ()
{
}

void abstract_painter::set_configure_all (int value)
{
  for (int i = 0; i < CONFIGURE_TYPE__COUNT; i++)
    set_configure_needed ((configure_type)i, value);
}
