#ifndef TOOLS_CONTAINER_H
#define TOOLS_CONTAINER_H

#include <QObject>

#include <map>
#include <memory>

class svg_painter;
class abstract_tool;
class selector_tool;
class gui_actions;

enum class gui_action_id;

class tools_container : public QObject
{
  Q_OBJECT

  gui_action_id m_cur_tool;
  std::map<gui_action_id, std::unique_ptr<abstract_tool>> m_tools;

  gui_actions *m_actions;
public:
  tools_container (gui_actions *actions);
  ~tools_container ();

  void update_tools (svg_painter *painter);

  abstract_tool *current_tool () const;

signals:
  void tool_changed ();

private slots:
  void update_actions ();

private:
  void add_tool (gui_action_id id, abstract_tool *tool);
  void change_tool (gui_action_id cur_tool_action);
  void remove_tools ();
  void connect_action (gui_action_id id);
};

#endif // TOOLS_CONTAINER_H
