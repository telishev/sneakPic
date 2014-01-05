#ifndef STYLE_WIDGET_HANDLER_H
#define STYLE_WIDGET_HANDLER_H

#include <QObject>

#include <memory>

class dock_widget_builder;

class QButtonGroup;
class QColor;
class QComboBox;
class QDoubleSpinBox;
class QSignalMapper;
class QTabWidget;
class QVBoxLayout;
class QWidget;

class color_indicator;
class color_selector_widget_handler;
class connection;
class settings_t;
class style_controller;
class tools_container;
enum class selected_style;

const char *enum_to_string (selected_style value);

class style_widget_handler : public QObject
{
  Q_OBJECT

  dock_widget_builder *m_dock_widget_builder;
  color_selector_widget_handler *m_fill_color_selector_widget_handler;
  color_selector_widget_handler *m_stroke_color_selector_widget_handler;
  QVBoxLayout *m_stroke_style_layout;
  QWidget *m_widget;
  QVBoxLayout *m_layout;
  QVBoxLayout *m_style_selector_layout;
  QTabWidget *m_style_type_widget;
  color_indicator *m_fill_color_indicator;
  color_indicator *m_stroke_color_indicator;
  QColor *m_fill_placeholder_color; // For playing around while nothing is opened
  QColor *m_stroke_placeholder_color;

  QDoubleSpinBox *m_stroke_width_spinbox;
  QComboBox *m_stroke_linejoin_combobox;
  QDoubleSpinBox *m_stroke_miterlimit_spinbox;
  QComboBox *m_stroke_linecap_combobox;

  selected_style m_cur_target_style;
  QButtonGroup *m_target_style;
  QSignalMapper *m_target_style_mapper;
  std::unique_ptr <connection> m_target_items_changed_connection;
  const tools_container *m_tools_container;

  style_controller *m_style_controller;

public:
  style_widget_handler (dock_widget_builder *dock_widget_builder_arg, style_controller *style_controller_arg);
  ~style_widget_handler ();
  void set_style_controller (style_controller *style_controller_arg);
  void set_tools_containter(const tools_container *tools_container_arg);

private slots:
  void selected_style_changed ();
  void target_items_changed ();
  void update_on_tool_changed ();
  void update_linejoin (int index);
  void update_linecap (int index);

private:
  void update_style_controllers ();
};

#endif // STYLE_WIDGET_HANDLER_H

