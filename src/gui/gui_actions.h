#ifndef GUI_ACTIONS_H
#define GUI_ACTIONS_H

#include <QObject>
#include <vector>
#include <memory>

class shortcuts_config;
class actions_info;
class QAction;
class QSignalMapper;

enum class gui_action_id;

class gui_actions : public QObject
{
  Q_OBJECT

  const shortcuts_config *m_cfg;

  actions_info *m_info;
  std::vector<std::unique_ptr<QAction>> m_actions;
  QSignalMapper *m_mapper;

public:
  gui_actions (const shortcuts_config *cfg);
  ~gui_actions ();

  QAction *action (gui_action_id id) const;

  void update_actions ();

signals:
  void triggered (gui_action_id id);

private slots:
  void action_triggered (int id);
};

#endif // GUI_ACTIONS_H
