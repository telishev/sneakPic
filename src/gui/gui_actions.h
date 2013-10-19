#ifndef GUI_ACTIONS_H
#define GUI_ACTIONS_H

#include <QObject>
#include <vector>
#include <memory>

class shortcuts_config;
class actions_info;
class QAction;
class QTimer;

enum class gui_action_id;

class gui_actions : public QObject
{
  Q_OBJECT

  const shortcuts_config *m_cfg;

  actions_info *m_info;
  std::vector<std::unique_ptr<QAction>> m_actions;
public:
  gui_actions (const shortcuts_config *cfg);
  ~gui_actions ();

  QAction *action (gui_action_id id) const;

  void rebuild_actions ();
  void set_update_needed ();

signals:
  void actions_update_needed ();
};

#endif // GUI_ACTIONS_H
