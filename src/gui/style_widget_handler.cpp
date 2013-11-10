#include "gui/style_widget_handler.h"

#include "common/memory_deallocation.h"

#include "editor/fill_style.h"
#include "editor/style_container.h"
#include "editor/style_controller.h"

#include "gui/color_selectors/color_indicator.h"
#include "gui/color_selector_widget_handler.h"
#include "gui/connection.h"
#include "gui/dock_widget_builder.h"
#include "gui/utils/qt_utils.h"

#include <QButtonGroup>
#include <QRadioButton>
#include <QSignalMapper>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QWidget>

#include <stddef.h>

static int TARGET_STYLE_ROLE = Qt::UserRole;

style_widget_handler::style_widget_handler (dock_widget_builder *dock_widget_builder_arg, style_controller *style_controller_arg)
{
  m_dock_widget_builder = dock_widget_builder_arg;
  m_widget = new QWidget ();
  m_widget->setObjectName ("Style");
  m_placeholder_color = new QColor ();
  m_layout = new QVBoxLayout (m_widget);
  m_color_selector_widget_handler = new color_selector_widget_handler (m_placeholder_color);
  m_color_indicator = new color_indicator (m_widget, m_placeholder_color);
  m_cur_target_style = selected_style::EDITOR_STYLE;
  m_style_controller = style_controller_arg;
  CONNECT (m_color_selector_widget_handler, &color_selector_widget_handler::color_changed_momentarily, m_color_indicator, &color_selector::color_changed_externally);
  CONNECT (m_color_selector_widget_handler, &color_selector_widget_handler::color_changed_momentarily, m_style_controller, &style_controller::update_color_momentarily);
  CONNECT (m_color_selector_widget_handler, &color_selector_widget_handler::color_changing_finished, m_style_controller, &style_controller::apply_changes);
  CONNECT (m_style_controller, &style_controller::controller_updates_needed, this, &style_widget_handler::update_color_in_controllers);

  m_layout->addWidget (m_color_indicator);

  m_target_style = new QButtonGroup (m_widget);
  m_target_style_mapper = new QSignalMapper (m_target_style);
  QHBoxLayout *m_target_style_layout = create_inner_hbox_layout (m_widget);
  for (int i = 0; i < (int) selected_style::COUNT; i++)
    {
      QRadioButton *button = new QRadioButton (enum_to_string ((selected_style) i), m_widget);
      if ((selected_style) i == m_cur_target_style)
        button->setChecked (true);
      m_target_style->addButton (button, i);
      m_target_style_layout->addWidget (button);
    }
  m_target_style->button ((int) m_style_controller->current_style ())->setChecked (true);
  connect (m_target_style, (void (QButtonGroup::*) (int)) &QButtonGroup::buttonClicked, this, &style_widget_handler::target_style_changed);

  finish_with_spacer (m_target_style_layout);

  m_style_type_widget = new QTabWidget (m_widget);
  m_layout->addWidget (m_style_type_widget);

  m_style_type_widget->addTab (m_color_selector_widget_handler->widget (), QIcon (), "Fill");
  m_dock_widget_builder->add_widget (m_widget, Qt::RightDockWidgetArea, visibility_state::visible, Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

  target_style_changed ();
};

void style_widget_handler::target_style_changed ()
{
  if (!m_style_controller)
    return;

  selected_style checked = (selected_style) m_target_style->checkedId ();

  if (checked != m_style_controller->current_style ()) // synchronize with radio button
    m_style_controller->switch_to (checked);

  update_color_in_controllers();
}

style_widget_handler::~style_widget_handler ()
{
  FREE (m_placeholder_color);
}

void style_widget_handler::update_color_in_controllers ()
{
  QColor *color =  m_style_controller->active_container ()->get_fill_style ()->color ();
  m_color_selector_widget_handler->set_color (color);
  m_color_indicator->set_color (color);
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
