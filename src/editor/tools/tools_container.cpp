#include "editor/tools/tools_container.h"

#include <QAction>

#include "common/memory_deallocation.h"

#include "editor/tools/selector_tool.h"

#include "gui/gui_action_id.h"
#include "gui/gui_actions.h"


tools_container::tools_container (gui_actions *actions)
{
  m_actions = actions;
  m_cur_tool = gui_action_id::TOOL_SELECTOR;

  connect (m_actions, SIGNAL (actions_update_needed ()), this, SLOT (update_actions ()));
  connect_action (gui_action_id::TOOL_SELECTOR);
}

tools_container::~tools_container ()
{
  remove_tools ();
}

void tools_container::update_tools (svg_painter *painter)
{
  remove_tools ();
  add_tool (gui_action_id::TOOL_SELECTOR, new selector_tool (painter));
  m_cur_tool = gui_action_id::TOOL_SELECTOR;
  update_actions ();
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

void tools_container::connect_action (gui_action_id id)
{
  connect (m_actions->action (id), &QAction::triggered, [this, id] () { change_tool (id); });
}
