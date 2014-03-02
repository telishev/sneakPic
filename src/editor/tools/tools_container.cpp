#include "editor/tools/tools_container.h"

#include <QAction>

#include "common/memory_deallocation.h"

#include "editor/tools/ellipse_tool.h"
#include "editor/tools/selector_tool.h"
#include "editor/tools/rectangle_tool.h"
#include "editor/tools/pen_tool.h"

#include "gui/gui_action_id.h"
#include "gui/gui_actions.h"
#include "gui/connection.h"

#include "path_editor_tool.h"



tools_container::tools_container (gui_actions *actions)
{
  m_cur_tool = gui_action_id::TOOL_SELECTOR;
  m_actions = actions;

  init_tools_info ();

  if (actions)
    CONNECT (m_actions, &gui_actions::actions_update_needed, this, &tools_container::update_actions);
}

tools_container::~tools_container ()
{
  remove_tools ();
}

void tools_container::update_tools (svg_painter *painter)
{
  remove_tools ();
  for (const auto &info_pair : m_tools_info)
    add_tool (info_pair.first, info_pair.second (painter));

  m_cur_tool = gui_action_id::TOOL_SELECTOR;
  update_actions ();
}

gui_action_id tools_container::current_tool_id () const
{
  return m_cur_tool;
}

abstract_tool *tools_container::current_tool () const
{
  return m_tools.at (m_cur_tool).get ();
}

void tools_container::remove_tools ()
{
  m_tools.clear ();
}

void tools_container::change_tool (gui_action_id cur_tool)
{
  if (cur_tool == m_cur_tool)
    {
      update_actions ();
      return;
    }

  m_cur_tool = cur_tool;
  m_actions->set_update_needed ();
  emit tool_changed ();
}

void tools_container::update_actions ()
{
  for (const auto &tools_pair : m_tools)
    {
      gui_action_id id = tools_pair.first;
      m_actions->action (id)->setCheckable (true);
      m_actions->action (id)->setChecked (id == m_cur_tool);
    }
}

void tools_container::add_tool (gui_action_id id, abstract_tool *tool)
{
  m_tools[id].reset (tool);
}

vector<gui_action_id> tools_container::tool_actions () const
{
  vector<gui_action_id> result;
  for (auto tool_pair : m_tools_info)
    result.push_back (tool_pair.first);

  return result;
}

template<typename T>
void tools_container::add_info (gui_action_id id)
{
  m_tools_info[id] = [] (svg_painter *painter) { return new T (painter); };
}

void tools_container::init_tools_info ()
{
  add_info<selector_tool> (gui_action_id::TOOL_SELECTOR);
  add_info<path_editor_tool> (gui_action_id::TOOL_PATH_EDITOR);
  add_info<rectangle_tool> (gui_action_id::TOOL_RECTANGLE);
  add_info<ellipse_tool> (gui_action_id::TOOL_ELLIPSE);
  add_info<pen_tool> (gui_action_id::TOOL_PEN);
}

bool tools_container::action_triggered (gui_action_id id)
{
  if (m_tools_info.find (id) == m_tools_info.end ())
    return false;

  change_tool (id);
  return true;
}
