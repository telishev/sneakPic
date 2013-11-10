#ifndef STYLE_WIDGET_HANDLER_H
#define STYLE_WIDGET_HANDLER_H

#include <QObject>

class dock_widget_builder;

class QButtonGroup;
class QColor;
class QSignalMapper;
class QTabWidget;
class QVBoxLayout;
class QWidget;

class color_indicator;
class color_selector_widget_handler;
class settings_t;
class style_controller;
enum class selected_style;

const char *enum_to_string (selected_style value);

class style_widget_handler : public QObject
{
  Q_OBJECT

  dock_widget_builder *m_dock_widget_builder;
  color_selector_widget_handler *m_color_selector_widget_handler;
  QWidget *m_widget;
  QVBoxLayout *m_layout;
  QTabWidget *m_style_type_widget;
  color_indicator *m_color_indicator;
  QColor *m_placeholder_color; // For playing around while nothing is opened

  selected_style m_cur_target_style;
  QButtonGroup *m_target_style;
  QSignalMapper *m_target_style_mapper;

  style_controller *m_style_controller;

public:
  style_widget_handler (dock_widget_builder *dock_widget_builder_arg, style_controller *style_controller_arg);
  ~style_widget_handler ();
  void set_style_controller (style_controller *style_controller_arg);

private slots:
  void target_style_changed();

public slots:
  void update_color_in_controllers ();
};

#endif // STYLE_WIDGET_HANDLER_H

