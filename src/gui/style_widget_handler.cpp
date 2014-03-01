#include "gui/style_widget_handler.h"

#include <QButtonGroup>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QRadioButton>
#include <QSignalMapper>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QWidget>
#include <QPushButton>

#include <stddef.h>

#include "common/memory_deallocation.h"

#include "editor/style_controller.h"
#include "editor/tools/tools_container.h"

#include "gui/color_selector_widget_handler.h"
#include "gui/color_selectors/color_indicator.h"
#include "gui/connection.h"
#include "gui/dock_widget_builder.h"
#include "gui/gui_action_id.h"
#include "gui/utils/qt_utils.h"
#include "gui/stroke_config_widget.h"


style_widget_handler::style_widget_handler (dock_widget_builder *dock_widget_builder_arg, style_controller *style_controller_arg)
{
  m_dock_widget_builder = dock_widget_builder_arg;
  m_style_controller = style_controller_arg;
  m_color_selector = new color_selector_widget_handler (m_style_controller);
  m_stroke_config = new stroke_config_widget (m_style_controller);

  m_dock_widget_builder->add_widget (m_color_selector, Qt::RightDockWidgetArea, visibility_state::visible, Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  m_dock_widget_builder->add_widget (m_stroke_config, Qt::RightDockWidgetArea, visibility_state::visible, Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
};

style_widget_handler::~style_widget_handler ()
{
}

void style_widget_handler::update_on_tool_changed ()
{
  gui_action_id id = m_tools_container->current_tool_id ();
  switch (id)
    {
    case gui_action_id::TOOL_SELECTOR:
    case gui_action_id::TOOL_PATH_EDITOR:
      m_style_controller->set_current_style (selected_style::SELECTED_STYLE);
      break;
    case gui_action_id::TOOL_RECTANGLE:
    case gui_action_id::TOOL_PEN:
      m_style_controller->set_current_style (selected_style::EDITOR_STYLE);
      break;
    default:
      break;
    };
}

void style_widget_handler::set_tools_containter (const tools_container *tools_container_arg)
{
  m_tools_container = tools_container_arg;
  CONNECT (m_tools_container, &tools_container::tool_changed, this, &style_widget_handler::update_on_tool_changed);
  update_on_tool_changed ();
}

const char *enum_to_string (selected_style value)
{
  switch (value)
    {
    case selected_style::EDITOR_STYLE:
      return "New Objects";
    case selected_style::SELECTED_STYLE:
      return "Selected Objects";
    case selected_style::COUNT:
      return "";
    }
  return "";
}

