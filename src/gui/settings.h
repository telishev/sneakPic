#ifndef SETTINGS_H
#define SETTINGS_H

class shortcuts_config;

class QColor;

class settings_t
{
  shortcuts_config *m_shortcuts;
  QColor *m_fill_color;
  QColor *m_stroke_color;
  double m_line_width;
public:
  settings_t ();
  ~settings_t ();

  shortcuts_config *shortcuts_cfg () const { return m_shortcuts; }
  QColor *fill_color () const { return m_fill_color; }
  QColor *stroke_color () const { return m_stroke_color; }
  double stroke_width () const { return m_line_width; }
  void set_stroke_width ( double value );
};

#endif // SETTINGS_H
