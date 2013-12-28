#ifndef TOOLS_CONTAINER_H
#define TOOLS_CONTAINER_H

#include <QObject>

#include <map>
#include <memory>
#include <functional>
#include <vector>

class svg_painter;
class abstract_tool;
class selector_tool;
class gui_actions;
class connection;

enum class gui_action_id;

class tools_container : public QObject
{
  Q_OBJECT

  gui_action_id m_cur_tool;
  typedef std::map<gui_action_id, std::function<abstract_tool * (svg_painter *)>> info_map_t;
  info_map_t m_tools_info;
  std::map<gui_action_id, std::unique_ptr<abstract_tool>> m_tools;

  gui_actions *m_actions;
public:
  tools_container (gui_actions *actions);
  ~tools_container ();

  void update_tools (svg_painter *painter);

  abstract_tool *current_tool () const;
  gui_action_id current_tool_id () const;
  std::vector<gui_action_id> tool_actions () const;
  bool action_triggered (gui_action_id id);

signals:
  void tool_changed ();

private slots:
  void update_actions ();

private:
  void add_tool (gui_action_id id, abstract_tool *tool);
  void change_tool (gui_action_id cur_tool_action);
  void remove_tools ();
  void connect_action (gui_action_id id);
  void init_tools_info ();

  template<typename T>
  void add_info (gui_action_id id);

};

#endif // TOOLS_CONTAINER_H
