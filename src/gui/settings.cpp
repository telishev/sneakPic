#include "gui/settings.h"

#include "common/memory_deallocation.h"

#include "gui/shortcuts_config.h"

settings_t::settings_t ()
{
  m_shortcuts = new shortcuts_config;
  m_shortcuts->fill_by_default ();
}

settings_t::~settings_t ()
{
  FREE (m_shortcuts);
}
