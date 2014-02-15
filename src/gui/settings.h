#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>

class shortcuts_config;

class QColor;

enum class option_id
{
  NEXT_CURVE_PREVIEW,
};

const char *enum_to_string (option_id value);
class settings_t : public QObject
{
  Q_OBJECT

  shortcuts_config *m_shortcuts;
  QColor *m_fill_color;
  QColor *m_stroke_color;
  double m_stroke_width;
  Qt::PenJoinStyle m_stroke_linejoin;
  double m_stroke_miterlimit;
  Qt::PenCapStyle m_stroke_linecap;
  QSettings *m_qsettings;
  vector <QVariant> m_storage;

public:
  settings_t (QSettings *qsettings);
  ~settings_t ();

  shortcuts_config *shortcuts_cfg () const { return m_shortcuts; }
  QColor *fill_color () const { return m_fill_color; }
  QColor *stroke_color () const { return m_stroke_color; }
  double stroke_width () const { return m_stroke_width; }
  Qt::PenJoinStyle stroke_linejoin () const { return m_stroke_linejoin; }
  Qt::PenCapStyle stroke_linecap () const {return m_stroke_linecap; }
  double stroke_miterlimit () const { return m_stroke_miterlimit; }

  void set_stroke_width (double value);
  void set_stroke_linejoin (Qt::PenJoinStyle value);
  void set_stroke_miterlimit (double value);
  void set_stroke_linecap (Qt::PenCapStyle value) { m_stroke_linecap = value; }
  QVariant value (option_id arg);
  void save ();
  void load ();
  void set_value (option_id arg, QVariant value);
  static const char *user_string (option_id arg);
  void settings_updated ();

signals:
  void settings_changed ();
};

#endif // SETTINGS_H
