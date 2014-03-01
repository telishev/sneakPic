#include "gui/settings.h"

#include "common/enum_conversion.h"
#include "common/memory_deallocation.h"

#include "gui/shortcuts_config.h"

#include <QColor>

settings_t::settings_t (QSettings *qsettings)
{
  m_shortcuts = new shortcuts_config;
  m_shortcuts->fill_by_default ();
  m_qsettings = qsettings;
  m_storage.resize (enum_count <option_id> ());
}

settings_t::~settings_t ()
{
  FREE (m_shortcuts);
}

const char *enum_to_string (option_id arg)
{
  switch (arg)
  {
  case option_id::NEXT_CURVE_PREVIEW:
    return "NextCurvePreview";
  }
  return "";
}

const char *settings_t::user_string (option_id arg)
{
  switch (arg)
  {
  case option_id::NEXT_CURVE_PREVIEW:
    return "Next Curve Preview";
  }
  return "";
}

QVariant default_value (option_id arg)
{
  switch (arg)
  {
  case option_id::NEXT_CURVE_PREVIEW:
    return false;
  }
  return QVariant ();
}

QVariant settings_t::value (option_id arg)
{
  return m_storage[(int) arg];
}

void settings_t::set_value (option_id arg, QVariant value)
{
  m_storage[(int) arg] = value;
}

void settings_t::save ()
{
  for (int i = 0; i < enum_count<option_id> (); i++)
    m_qsettings->setValue (enum_to_string ((option_id) i), m_storage[i]);
}

void settings_t::load ()
{
  for (int i = 0; i < enum_count<option_id> (); i++)
    m_storage[i] = m_qsettings->value (enum_to_string ((option_id) i), default_value ((option_id) i));
}

void settings_t::settings_updated ()
{
  emit settings_changed ();
}
