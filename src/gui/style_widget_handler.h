#ifndef STYLE_WIDGET_HANDLER_H
#define STYLE_WIDGET_HANDLER_H

#include <QObject>
#include <QColor>

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
class stroke_config_widget;
enum class selected_style;

const char *enum_to_string (selected_style value);

class style_widget_handler : public QObject
{
  Q_OBJECT

  dock_widget_builder *m_dock_widget_builder;
  color_selector_widget_handler *m_color_selector;

  const tools_container *m_tools_container;
  style_controller *m_style_controller;
  stroke_config_widget *m_stroke_config;

public:
  style_widget_handler (dock_widget_builder *dock_widget_builder_arg, style_controller *style_controller_arg);
  ~style_widget_handler ();

  void set_style_controller (style_controller *style_controller_arg);
  void set_tools_containter (const tools_container *tools_container_arg);

private slots:
  void update_on_tool_changed ();
};

#endif // STYLE_WIDGET_HANDLER_H

