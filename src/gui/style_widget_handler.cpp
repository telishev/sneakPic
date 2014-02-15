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

#include <QButtonGroup>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QRadioButton>
#include <QSignalMapper>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QWidget>

#include <stddef.h>

#include <QPushButton>

Q_DECLARE_METATYPE (Qt::PenJoinStyle);
Q_DECLARE_METATYPE (Qt::PenCapStyle);

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
  m_cur_target_style = selected_style::EDITOR_STYLE;
  m_style_controller = style_controller_arg;
  CONNECT (m_fill_color_selector_widget_handler, &color_selector_widget_handler::color_changed_momentarily, this, &style_widget_handler::fill_color_changed);
  m_controller_connections.push_back (CONNECT (m_fill_color_selector_widget_handler, &color_selector_widget_handler::color_changed_momentarily, m_style_controller, &style_controller::update_fill_color_momentarily));
  CONNECT (m_fill_color_selector_widget_handler, &color_selector_widget_handler::color_changing_finished, m_style_controller, &style_controller::apply_changes);

  m_stroke_color_selector_widget_handler = new color_selector_widget_handler (m_fill_placeholder_color);
  CONNECT (m_stroke_color_selector_widget_handler, &color_selector_widget_handler::color_changed_momentarily, this, &style_widget_handler::stroke_color_changed);
  m_controller_connections.push_back (CONNECT (m_stroke_color_selector_widget_handler, &color_selector_widget_handler::color_changed_momentarily, m_style_controller, &style_controller::update_stroke_color_momentarily));
  CONNECT (m_stroke_color_selector_widget_handler, &color_selector_widget_handler::color_changing_finished, m_style_controller, &style_controller::apply_changes);

  m_target_items_changed_connection = CONNECT (m_style_controller, &style_controller::target_items_changed, this, &style_widget_handler::target_items_changed);

  init_target_style_controller ();

  m_style_type_widget = new QTabWidget (m_widget);
  m_layout->addWidget (m_style_type_widget);

  {
    QVBoxLayout *layout = create_common_vbox_widget ();
    layout->addWidget (new QLabel ("Color:"));
    layout->addWidget (m_fill_color_selector_widget_handler->widget ());
    layout->addStretch ();
    m_style_type_widget->addTab (layout->parentWidget (), QIcon (), "Fill");
  }

  m_stroke_style_layout = create_common_vbox_widget (nullptr);
  m_stroke_style_layout->addWidget (new QLabel ("Color:"));
  m_stroke_style_layout->addWidget (m_stroke_color_selector_widget_handler->widget ());

  init_stroke_width_controller ();
  init_linejoin_controller();
  init_miterlimit_controller();
  init_linecap_controller();

  m_stroke_style_layout->addStretch ();
  m_style_type_widget->addTab (m_stroke_style_layout->parentWidget (), QIcon (), "Stroke");

  m_dock_widget_builder->add_widget (m_widget, Qt::RightDockWidgetArea, visibility_state::visible, Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

  selected_style_changed ();
};

void style_widget_handler::update_linejoin (int index)
{
  m_style_controller->update_linejoin (m_stroke_linejoin_combobox->itemData (index).value <Qt::PenJoinStyle> ());
}

void style_widget_handler::update_linecap (int index)
{
  m_style_controller->update_linecap (m_stroke_linecap_combobox->itemData (index).value <Qt::PenCapStyle> ());
}

void style_widget_handler::send_color_changes ()
{
  QColor lacuna (127, 127, 127, 127);
  if (m_style_controller->active_container ()->get_fill_style ()->color ())
    emit fill_color_changed (*m_style_controller->active_container ()->get_fill_style ()->color ());
  else
    emit fill_color_changed (lacuna);

  if (m_style_controller->active_container ()->get_stroke_style ()->color ())
    emit stroke_color_changed (*m_style_controller->active_container ()->get_stroke_style ()->color ());
  else
    emit stroke_color_changed (lacuna);
}

void style_widget_handler::target_items_changed ()
{
  TEMPORARY_DISCONNECT (m_target_items_changed_connection);

  for (const auto &_connection : m_controller_connections)
    _connection->disconnect ();

  update_style_controllers ();
  send_color_changes ();

  m_fill_color_selector_widget_handler->update_colors_momentarily ();
  m_stroke_color_selector_widget_handler->update_colors_momentarily ();

  for (const auto &_connection : m_controller_connections)
    _connection->connect ();
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

  color =  m_style_controller->active_container ()->get_stroke_style ()->color ();
  m_stroke_color_selector_widget_handler->set_color (color);

  send_color_changes ();

  m_stroke_width_spinbox->setValue (m_style_controller->stroke_width ());
  m_stroke_miterlimit_spinbox->setValue (m_style_controller->stroke_miterlimit ());

  {
    Qt::PenJoinStyle join_style = m_style_controller->stroke_linejoin ();
    int index = m_stroke_linejoin_combobox->findData (QVariant::fromValue<Qt::PenJoinStyle> (join_style));
    m_stroke_linejoin_combobox->setCurrentIndex (index);

    m_stroke_miterlimit_spinbox->setEnabled (join_style == Qt::PenJoinStyle::SvgMiterJoin);
  }

  {
    Qt::PenCapStyle cap_style = m_style_controller->stroke_linecap ();
    int index = m_stroke_linecap_combobox->findData (QVariant::fromValue<Qt::PenCapStyle> (cap_style));
    m_stroke_linecap_combobox->setCurrentIndex (index);
  }
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
    case gui_action_id::TOOL_PEN:
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

void style_widget_handler::init_linejoin_controller ()
{
  {
    QHBoxLayout *layout = create_intermediate_hbox_layout (m_stroke_style_layout);
    layout->addWidget (new QLabel ("Join:"));
    layout->addWidget (m_stroke_linejoin_combobox = new QComboBox (layout->parentWidget ()));
    layout->addStretch ();
    m_stroke_linejoin_combobox->addItem (QIcon (":/linejoin_miter.png"), "Miter", QVariant::fromValue <Qt::PenJoinStyle> (Qt::PenJoinStyle::SvgMiterJoin));
    m_stroke_linejoin_combobox->addItem (QIcon (":/linejoin_round.png"), "Round", QVariant::fromValue <Qt::PenJoinStyle> (Qt::PenJoinStyle::RoundJoin));
    m_stroke_linejoin_combobox->addItem (QIcon (":/linejoin_bevel.png"), "Bevel", QVariant::fromValue <Qt::PenJoinStyle> (Qt::PenJoinStyle::BevelJoin));
    m_controller_connections.push_back (CONNECT (m_stroke_linejoin_combobox, (void (QComboBox::*) (int)) &QComboBox::currentIndexChanged, this, &style_widget_handler::update_linejoin));
  }
}

void style_widget_handler::init_miterlimit_controller ()
{
  {
    QHBoxLayout *layout = create_intermediate_hbox_layout (m_stroke_style_layout);
    layout->addWidget (new QLabel ("Miter limit:"));
    layout->addWidget (m_stroke_miterlimit_spinbox = create_double_spinbox (layout->parentWidget (), 100.0, 2));
    m_controller_connections.push_back (CONNECT (m_stroke_miterlimit_spinbox, (void (QDoubleSpinBox::*) (double)) &QDoubleSpinBox::valueChanged, m_style_controller, &style_controller::update_stroke_miterlimit));
    layout->addStretch ();
  }
}

void style_widget_handler::init_stroke_width_controller ()
{
  {
    QHBoxLayout *layout = create_intermediate_hbox_layout (m_stroke_style_layout);
    layout->addWidget (new QLabel ("Width:"));
    layout->addWidget (m_stroke_width_spinbox = create_double_spinbox (layout->parentWidget (), 1000.0));
    m_controller_connections.push_back (CONNECT (m_stroke_width_spinbox, (void (QDoubleSpinBox::*) (double)) &QDoubleSpinBox::valueChanged, m_style_controller, &style_controller::update_stroke_width));
    layout->addStretch ();
  }
}

void style_widget_handler::init_linecap_controller ()
{
  {
    QHBoxLayout *layout = create_intermediate_hbox_layout (m_stroke_style_layout);
    layout->addWidget (new QLabel ("Cap:"));
    layout->addWidget (m_stroke_linecap_combobox = new QComboBox (layout->parentWidget ()));
    layout->addStretch ();
    m_stroke_linecap_combobox->addItem (QIcon (":/linecap_butt.png"), "Butt", QVariant::fromValue <Qt::PenCapStyle> (Qt::PenCapStyle::FlatCap));
    m_stroke_linecap_combobox->addItem (QIcon (":/linecap_round.png"), "Round", QVariant::fromValue <Qt::PenCapStyle> (Qt::PenCapStyle::RoundCap));
    m_stroke_linecap_combobox->addItem (QIcon (":/linecap_cap.png"), "Cap", QVariant::fromValue <Qt::PenCapStyle> (Qt::PenCapStyle::SquareCap));
    m_controller_connections.push_back (CONNECT (m_stroke_linecap_combobox, (void (QComboBox::*) (int)) &QComboBox::currentIndexChanged, this, &style_widget_handler::update_linecap));
  }
}

void style_widget_handler::init_target_style_controller ()
{
  m_target_style = new QButtonGroup (m_widget);
  m_target_style_mapper = new QSignalMapper (m_target_style);
  QHBoxLayout *m_target_style_layout = create_intermediate_hbox_layout (m_widget);
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

