#include "gui/tools_widget_builder.h"

#include <QDockWidget>
#include <QGridLayout>
#include <QMainWindow>
#include <QToolBar>
#include <QToolButton>

#include "common/memory_deallocation.h"

#include "gui/gui_action_id.h"
#include "gui/gui_actions.h"
#include "gui/color_selectors.h"
#include "gui/dock_widget_builder.h"
#include "editor/tools/tools_container.h"

static int NUMBER_OF_COLUMNS = 3;
static int MAXIMUM_WIDTH = 100;

tools_widget_builder::tools_widget_builder (gui_actions *actions, dock_widget_builder *dock_widget_builder_arg)
{
  m_actions = actions;
  m_tool_bar = 0;
  m_dock_widget_builder = dock_widget_builder_arg;
  update ();
}

tools_widget_builder::~tools_widget_builder ()
{
}

void tools_widget_builder::update ()
{
  FREE (m_tool_bar);
  init_toolbar_and_layout();
  fill_tool_bar ();
}

void tools_widget_builder::fill_tool_bar ()
{
  m_count = 0;
  tools_container container (nullptr);
  for (gui_action_id id : container.tool_actions ())
    {
      add_action (id);
    }
  m_layout->addItem (new QSpacerItem (0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding), m_layout->rowCount (), 0);
}

void tools_widget_builder::add_action (gui_action_id id)
{
  QToolButton *tool_button = new QToolButton (m_tool_bar);
  tool_button->setAutoRaise (true);
  tool_button->setCheckable (true);
  tool_button->setDefaultAction (m_actions->action (id));
  m_layout->addWidget (tool_button, m_count / NUMBER_OF_COLUMNS, m_count % NUMBER_OF_COLUMNS);
  m_count++;
}

void tools_widget_builder::init_toolbar_and_layout()
{
  m_tool_bar = new QWidget (0);
  m_layout = new QGridLayout ();
  m_layout->setSpacing (0);
  m_layout->setVerticalSpacing (0);
  m_layout->setHorizontalSpacing (0);
  m_layout->setMargin (0);
  m_tool_bar->setLayout (m_layout);
  m_tool_bar->setMaximumWidth (MAXIMUM_WIDTH);
  m_tool_bar->setWindowTitle ("Tools");
  m_dock_widget_builder->add_widget (m_tool_bar, Qt::LeftDockWidgetArea, visibility_state::visible);
}
