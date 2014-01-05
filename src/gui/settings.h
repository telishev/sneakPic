#ifndef SETTINGS_H
#define SETTINGS_H

class shortcuts_config;

class QColor;

class settings_t
{
  shortcuts_config *m_shortcuts;
  QColor *m_fill_color;
  QColor *m_stroke_color;
  double m_stroke_width;
  Qt::PenJoinStyle m_stroke_linejoin;
  double m_stroke_miterlimit;
  Qt::PenCapStyle m_stroke_linecap;

public:
  settings_t ();
  ~settings_t ();

  shortcuts_config *shortcuts_cfg () const { return m_shortcuts; }
  QColor *fill_color () const { return m_fill_color; }
  QColor *stroke_color () const { return m_stroke_color; }
  double stroke_width () const { return m_stroke_width; }
  Qt::PenJoinStyle stroke_linejoin () const { return m_stroke_linejoin; }
  double stroke_miterlimit () const { return m_stroke_miterlimit; }
  void set_stroke_width (double value);
  void set_stroke_linejoin (Qt::PenJoinStyle value);
  void set_stroke_miterlimit (double value);
  void set_stroke_linecap (Qt::PenCapStyle value) { m_stroke_linecap = value; }
  Qt::PenCapStyle stroke_linecap () {return m_stroke_linecap; }
};

#endif // SETTINGS_H
