#include "gui/tools_widget_builder.h"

#include <QDockWidget>
#include <QGridLayout>
#include <QLabel>
#include <QMainWindow>
#include <QToolBar>
#include <QToolButton>

#include "common/memory_deallocation.h"

#include "color_selectors/color_indicator.h"

#include "editor/tools/tools_container.h"

#include "gui/color_selectors/color_selector.h"
#include "gui/dock_widget_builder.h"
#include "gui/gui_action_id.h"
#include "gui/gui_actions.h"
#include "gui/utils/flowlayout.h"
#include "gui/utils/qt_utils.h"
#include "fill_stroke_widget.h"

// TODO: probably rewrite all color controllers on directly signal/slot signal specifying QColor

tools_widget_builder::tools_widget_builder (gui_actions *actions, dock_widget_builder *dock_widget_builder_arg, gui_model<style_controller_role_t> *model)
{
  m_actions = actions;
  m_tool_bar = nullptr;
  m_dock_widget_builder = dock_widget_builder_arg;
  m_fill_stroke = new fill_stroke_widget (model);
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
  add_color_indicators ();
  m_layout->addStretch ();
}

void tools_widget_builder::fill_tool_bar ()
{
  m_count = 0;
  tools_container container (nullptr);
  for (gui_action_id id : container.tool_actions ())
    {
      add_action (id);
    }
}

void tools_widget_builder::add_action (gui_action_id id)
{
  QToolButton *tool_button = new QToolButton (m_tool_bar);
  tool_button->setAutoRaise (true);
  tool_button->setCheckable (true);
  tool_button->setDefaultAction (m_actions->action (id));
  m_tool_bar->layout ()->addWidget (tool_button);
  m_count++;
}

void tools_widget_builder::init_toolbar_and_layout()
{
  m_layout = qt_utils::create_common_vbox_widget ();
  m_tool_bar = new QWidget (0);
  FlowLayout *layout = new FlowLayout ();
  layout->setSpacing (0);
  layout->setMargin (0);
  m_tool_bar->setLayout (layout);
  m_layout->addWidget (m_tool_bar);
  m_layout->parentWidget ()->setObjectName ("Tools");
  m_dock_widget_builder->add_widget (m_layout->parentWidget (), Qt::LeftDockWidgetArea, visibility_state::visible);
}

void tools_widget_builder::add_color_indicators ()
{
  QHBoxLayout *layout = qt_utils::create_intermediate_hbox_layout (m_layout->parentWidget ());
  layout->addWidget (m_fill_stroke);
  layout->setSpacing (3);
  m_layout->addWidget (layout->parentWidget ());
}
