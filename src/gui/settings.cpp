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
  m_stroke_linejoin = Qt::PenJoinStyle::SvgMiterJoin;
  m_stroke_width = 0.0;
}

settings_t::~settings_t ()
{
  FREE (m_shortcuts);
  FREE (m_fill_color);
  FREE (m_stroke_color);
}

void settings_t::set_stroke_width (double value)
{
  m_stroke_width = value;
}

void settings_t::set_stroke_linejoin (Qt::PenJoinStyle value)
{
  m_stroke_linejoin = value;
}
