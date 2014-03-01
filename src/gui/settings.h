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
  QSettings *m_qsettings;
  vector <QVariant> m_storage;

public:
  settings_t (QSettings *qsettings);
  ~settings_t ();

  shortcuts_config *shortcuts_cfg () const { return m_shortcuts; }

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
