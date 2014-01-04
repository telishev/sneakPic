#include "gui/style_widget_handler.h"

#include "common/memory_deallocation.h"

#include "editor/fill_style.h"
#include "editor/stroke_style.h"
#include "editor/style_container.h"
#include "editor/style_controller.h"
#include "editor/tools/tools_container.h"

#include "gui/color_selector_widget_handler.h"
#include "gui/color_selectors/color_indicator.h"
#include "gui/connection.h"
#include "gui/dock_widget_builder.h"
#include "gui/gui_action_id.h"
#include "gui/utils/qt_utils.h"
#include "gui/utils/override_undo_filter.h"

#include <QButtonGroup>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QRadioButton>
#include <QSignalMapper>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QWidget>

#include <stddef.h>

#include <QPushButton>

static int TARGET_STYLE_ROLE = Qt::UserRole;

style_widget_handler::style_widget_handler (dock_widget_builder *dock_widget_builder_arg, style_controller *style_controller_arg)
{
  m_dock_widget_builder = dock_widget_builder_arg;
  m_widget = new QWidget ();
  m_widget->setObjectName ("Style");
  m_fill_placeholder_color = new QColor ();
  m_stroke_placeholder_color = new QColor ();
  m_layout = new QVBoxLayout (m_widget);
  m_fill_color_selector_widget_handler = new color_selector_widget_handler (m_fill_placeholder_color);
  m_fill_color_indicator = new color_indicator (m_widget, m_fill_placeholder_color);
  m_stroke_color_indicator = new color_indicator (m_widget, m_stroke_placeholder_color);
  m_cur_target_style = selected_style::EDITOR_STYLE;
  m_style_controller = style_controller_arg;
  CONNECT (m_fill_color_selector_widget_handler, &color_selector_widget_handler::color_changed_momentarily, m_fill_color_indicator, &color_selector::color_changed_externally);
  CONNECT (m_fill_color_selector_widget_handler, &color_selector_widget_handler::color_changed_momentarily, m_style_controller, &style_controller::update_fill_color_momentarily);
  CONNECT (m_fill_color_selector_widget_handler, &color_selector_widget_handler::color_changing_finished, m_style_controller, &style_controller::apply_changes);

  m_stroke_color_selector_widget_handler = new color_selector_widget_handler (m_fill_placeholder_color);
  CONNECT (m_stroke_color_selector_widget_handler, &color_selector_widget_handler::color_changed_momentarily, m_stroke_color_indicator, &color_selector::color_changed_externally);
  CONNECT (m_stroke_color_selector_widget_handler, &color_selector_widget_handler::color_changed_momentarily, m_style_controller, &style_controller::update_stroke_color_momentarily);
  CONNECT (m_stroke_color_selector_widget_handler, &color_selector_widget_handler::color_changing_finished, m_style_controller, &style_controller::apply_changes);

  m_target_items_changed_connection = CONNECT (m_style_controller, &style_controller::target_items_changed, this, &style_widget_handler::target_items_changed);

  {
    QHBoxLayout *layout = create_inner_hbox_layout (m_widget);
    layout->addWidget (m_fill_color_indicator);
    layout->addWidget (m_stroke_color_indicator);
    layout->addStretch ();
  }

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
  CONNECT (m_target_style, (void (QButtonGroup::*) (int)) &QButtonGroup::buttonClicked, this, &style_widget_handler::selected_style_changed);
  m_target_style_layout->parentWidget ()->hide ();

  m_target_style_layout->addStretch ();

  m_style_type_widget = new QTabWidget (m_widget);
  m_layout->addWidget (m_style_type_widget);

  {
    QVBoxLayout *layout = create_common_vbox_layout ();
    layout->addWidget (m_fill_color_selector_widget_handler->widget ());
    layout->addStretch ();
    m_style_type_widget->addTab (layout->parentWidget (), QIcon (), "Fill");
  }

  m_stroke_style_layout = create_common_vbox_layout (nullptr);
  m_stroke_style_layout->addWidget (m_stroke_color_selector_widget_handler->widget ());

  {
    QHBoxLayout *layout = create_inner_hbox_layout (m_stroke_style_layout);
    layout->addWidget (new QLabel ("Width:"));
    layout->addWidget (m_stroke_width_spinbox = new QDoubleSpinBox (layout->parentWidget ()));
    m_stroke_width_spinbox->installEventFilter (new override_undo_filter (m_stroke_width_spinbox));
    m_stroke_width_spinbox->setSingleStep (0.1);
    m_stroke_width_spinbox->setMaximum (1000.0);
    m_stroke_width_spinbox->setDecimals (3);
    m_stroke_width_spinbox->setKeyboardTracking (false);
    CONNECT (m_stroke_width_spinbox, (void (QDoubleSpinBox::*) (double)) &QDoubleSpinBox::valueChanged, m_style_controller, &style_controller::update_line_width);
    layout->addStretch ();
  }

  m_stroke_style_layout->addStretch ();
  m_style_type_widget->addTab (m_stroke_style_layout->parentWidget (), QIcon (), "Stroke");

  m_dock_widget_builder->add_widget (m_widget, Qt::RightDockWidgetArea, visibility_state::visible, Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

  selected_style_changed ();
};

void style_widget_handler::target_items_changed ()
{
  TEMPORARY_DISCONNECT (m_target_items_changed_connection);
  update_style_controllers ();
  m_fill_color_indicator->color_changed_externally ();
  m_fill_color_selector_widget_handler->update_colors_momentarily ();
  m_stroke_color_indicator->color_changed_externally ();
  m_stroke_color_selector_widget_handler->update_colors_momentarily ();
}

void style_widget_handler::selected_style_changed ()
{
  if (!m_style_controller)
    return;

  selected_style checked = (selected_style) m_target_style->checkedId ();

  if (checked != m_style_controller->current_style ()) // synchronize with radio button
    m_style_controller->switch_to (checked);

  update_style_controllers ();
}

style_widget_handler::~style_widget_handler ()
{
  FREE (m_fill_placeholder_color);
}

void style_widget_handler::update_style_controllers ()
{
  QColor *color =  m_style_controller->active_container ()->get_fill_style ()->color ();
  m_fill_color_selector_widget_handler->set_color (color);
  m_fill_color_indicator->set_color (color);

  color =  m_style_controller->active_container ()->get_stroke_style ()->color ();
  m_stroke_color_selector_widget_handler->set_color (color);
  m_stroke_color_indicator->set_color (color);

  m_stroke_width_spinbox->setValue (m_style_controller->stroke_width ());
}

void style_widget_handler::update_on_tool_changed ()
{
  gui_action_id id = m_tools_container->current_tool_id ();
  switch (id)
    {
    case gui_action_id::TOOL_SELECTOR:
    case gui_action_id::TOOL_PATH_EDITOR:
      m_target_style->button ((int) selected_style::SELECTED_STYLE)->click ();
      break;
    case gui_action_id::TOOL_RECTANGLE:
      m_target_style->button ((int) selected_style::EDITOR_STYLE)->click ();
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

