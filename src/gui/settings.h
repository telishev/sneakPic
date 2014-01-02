#ifndef SETTINGS_H
#define SETTINGS_H

class shortcuts_config;

class QColor;

class settings_t
{
  shortcuts_config *m_shortcuts;
  QColor *m_fill_color;
  QColor *m_stroke_color;

public:
  settings_t ();
  ~settings_t ();

  shortcuts_config *shortcuts_cfg () const { return m_shortcuts; }
  QColor *fill_color () const { return m_fill_color; }
  QColor *stroke_color () const { return m_stroke_color; }
};

#endif // SETTINGS_H
