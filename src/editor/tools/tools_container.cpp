#include "editor/tools/tools_container.h"

#include "common/memory_deallocation.h"

#include "editor/tools/selector_tool.h"


tools_container::tools_container ()
{
  m_selector = nullptr;
  m_current_tool = nullptr;
}

tools_container::~tools_container ()
{
  remove_tools ();
}

void tools_container::update_tools (svg_painter *painter)
{
  remove_tools ();
  m_selector = new selector_tool (painter);

  m_current_tool = m_selector;
}

abstract_tool *tools_container::current_tool () const
{
  return m_current_tool;
}

void tools_container::remove_tools ()
{
  FREE (m_selector);
}
