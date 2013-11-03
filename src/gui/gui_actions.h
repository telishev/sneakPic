#ifndef GUI_ACTIONS_H
#define GUI_ACTIONS_H

#include <QObject>
#include <vector>
#include <memory>
#include <functional>
#include <map>

class shortcuts_config;
class actions_info;
class QAction;
class QSignalMapper;
class QShortcut;
class QWidget;

enum class gui_action_id;

class gui_actions : public QObject
{
  Q_OBJECT

  const shortcuts_config *m_cfg;

  actions_info *m_info;
  std::vector<std::unique_ptr<QAction>> m_actions;
  std::function<bool (gui_action_id)> m_callback;
  QSignalMapper *m_actions_mapper;
  QWidget *m_parent;

public:
  gui_actions (const shortcuts_config *cfg, std::function<bool (gui_action_id)> callback, QWidget *parent);
  ~gui_actions ();

  QAction *action (gui_action_id id) const;

  void rebuild_actions ();
  void set_update_needed ();

signals:
  void actions_update_needed ();

private slots:
  void action_triggered (int id);

private:
  void shortcut_triggered (QKeySequence sequnce);

  virtual bool eventFilter (QObject *watched, QEvent *event) override;
};

#endif // GUI_ACTIONS_H
