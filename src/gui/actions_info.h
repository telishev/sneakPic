#ifndef KEYBOARD_SHORTCUTS_INFO_H
#define KEYBOARD_SHORTCUTS_INFO_H

#include <QIcon>
#include <QString>
#include <QKeySequence>
#include <vector>

enum class gui_action_id;

class action_description
{
  QIcon m_icon;
  QString m_description;
  QKeySequence m_default_shortcut;

public:
  action_description ();
  action_description (const QString &description, const QIcon &icon, const QKeySequence &shortcut);

  ~action_description ();

  const QIcon &icon () const { return m_icon; }
  const QString &description () const { return m_description; }
  const QKeySequence &default_shortcut () const { return m_default_shortcut; }
};

class actions_info
{
  vector<action_description> m_info;
public:
  actions_info ();
  ~actions_info ();

  const QIcon &icon (gui_action_id id) const;
  const QString &description (gui_action_id id) const;
  const QKeySequence &default_shortcut (gui_action_id id) const;

private:
  void add_action (gui_action_id id, const QString &description, QKeySequence shortcut = QKeySequence (), QIcon icon = QIcon ());

  const action_description &descr (gui_action_id id) const;
};

#endif // KEYBOARD_SHORTCUTS_INFO_H
