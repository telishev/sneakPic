#ifndef SETTINGS_H
#define SETTINGS_H

class shortcuts_config;

class settings_t
{
  shortcuts_config *m_shortcuts;

public:
  settings_t ();
  ~settings_t ();

  shortcuts_config *shortcuts_cfg () const { return m_shortcuts; }
};

#endif // SETTINGS_H
