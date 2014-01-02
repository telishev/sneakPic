#include "editor/style_container.h"

#include "editor/fill_style.h"
#include "editor/stroke_style.h"

style_container::style_container ()
{
  m_fill_style = new fill_style ();
  m_stroke_style = new stroke_style ();
}

void style_container::init (settings_t *settings)
{
  m_fill_style->init (settings);
  m_stroke_style->init (settings);
}

void style_container::init (items_selection *selection)
{
  m_fill_style->init (selection);
  m_stroke_style->init (selection);
}
