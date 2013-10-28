#include "gui/tools_widget_builder.h"

#include <QDockWidget>
#include <QGridLayout>
#include <QMainWindow>
#include <QToolBar>
#include <QToolButton>

#include "common/memory_deallocation.h"

#include "gui/gui_action_id.h"
#include "gui/gui_actions.h"
#include "editor/tools/tools_container.h"

static int number_of_columns = 2;

tools_widget_builder::tools_widget_builder (gui_actions *actions, QMainWindow *main_window)
{
  m_actions = actions;
  m_main_window = main_window;
  m_tool_bar_window = nullptr;
  update ();
}

tools_widget_builder::~tools_widget_builder ()
{
  FREE (m_tool_bar_window);
}

void tools_widget_builder::update ()
{
  if (m_tool_bar_window)
    {
      m_main_window->removeDockWidget (m_tool_bar_window);
      FREE (m_tool_bar_window);
    }

  init_toolbar_and_layout();

  fill_tool_bar ();
  m_main_window->addDockWidget (Qt::LeftDockWidgetArea, m_tool_bar_window);
}

void tools_widget_builder::fill_tool_bar ()
{
  m_count = 0;
  tools_container container (nullptr);
  for (gui_action_id id : container.tool_actions ())
  {
    add_action (id);
  }
  m_layout->addItem (new QSpacerItem (0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding), m_count / number_of_columns + 1, 0);
}

void tools_widget_builder::add_action (gui_action_id id)
{
  QToolButton *tool_button = new QToolButton (m_tool_bar);
  tool_button->setAutoRaise (true);
  tool_button->setCheckable (true);
  tool_button->setDefaultAction (m_actions->action (id));
  m_layout->addWidget (tool_button, m_count / number_of_columns, m_count % number_of_columns);
  m_count++;
}

void tools_widget_builder::init_toolbar_and_layout()
{
  m_tool_bar_window = new QDockWidget ("Tools");
  m_tool_bar_window->setFeatures (QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable);
  m_tool_bar_window->setAllowedAreas (Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  m_tool_bar = new QWidget (m_tool_bar_window);
  m_layout = new QGridLayout ();
  m_layout->setSpacing (0);
  m_layout->setVerticalSpacing (0);
  m_layout->setHorizontalSpacing (0);
  m_layout->setMargin (0);
  m_tool_bar->setLayout (m_layout);
  m_tool_bar_window->setWidget (m_tool_bar);
}
