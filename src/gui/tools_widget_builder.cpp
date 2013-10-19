#include "gui/tools_widget_builder.h"

#include <QMainWindow>
#include <QToolBar>

#include "common/memory_deallocation.h"

#include "gui/gui_action_id.h"
#include "gui/gui_actions.h"




tools_widget_builder::tools_widget_builder (gui_actions *actions, QMainWindow *main_window)
{
  m_actions = actions;
  m_main_window = main_window;
  m_tool_bar = nullptr;
  update ();
}

tools_widget_builder::~tools_widget_builder ()
{
  FREE (m_tool_bar);
}

void tools_widget_builder::update ()
{
  if (m_tool_bar)
    {
      m_main_window->removeToolBar (m_tool_bar);
      FREE (m_tool_bar);
    }

  m_tool_bar = new QToolBar ("Tools");
  m_tool_bar->setIconSize (QSize (16, 16)); 
  m_tool_bar->setMovable (false);
  fill_tool_bar ();
  m_main_window->addToolBar (Qt::LeftToolBarArea, m_tool_bar);
}

void tools_widget_builder::fill_tool_bar ()
{
  add_action (gui_action_id::TOOL_SELECTOR);
}

void tools_widget_builder::add_action (gui_action_id id)
{
  m_tool_bar->addAction (m_actions->action (id));
}
