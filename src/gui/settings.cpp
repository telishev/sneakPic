#include "gui/settings.h"

#include "common/memory_deallocation.h"

#include "gui/shortcuts_config.h"

#include <QColor>

settings_t::settings_t ()
{
  m_shortcuts = new shortcuts_config;
  m_shortcuts->fill_by_default ();
  m_fill_color = new QColor ();
  m_stroke_color = new QColor ();
}

settings_t::~settings_t ()
{
  FREE (m_shortcuts);
  FREE (m_fill_color);
  FREE (m_stroke_color);
}
