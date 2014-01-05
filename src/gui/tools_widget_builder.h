#ifndef TOOLS_WIDGET_BUILDER_H
#define TOOLS_WIDGET_BUILDER_H

#include <QObject>

class QDockWidget;
class QMainWindow;
class QToolBar;
class QWidget;

class FlowLayout;

enum class gui_action_id;

class color_indicator;
class gui_actions;
class dock_widget_builder;

class QVBoxLayout;

class tools_widget_builder : public QObject
{
  Q_OBJECT

  gui_actions *m_actions;
  QMainWindow *m_main_window;
  QWidget *m_tool_bar;
  QVBoxLayout *m_layout;
  int m_count;
  dock_widget_builder *m_dock_widget_builder;
  color_indicator *m_fill_color_indicator;
  color_indicator *m_stroke_color_indicator;
  QColor *m_fill_color;
  QColor *m_stroke_color;

public:
  tools_widget_builder (gui_actions *actions, dock_widget_builder *dock_widget_builder_arg);
  ~tools_widget_builder ();

  void update ();

private:
  void fill_tool_bar ();
  void init_toolbar_and_layout();
  void add_action (gui_action_id id);
  void add_color_indicators();

public slots:
  void update_stroke_color (QColor &color);
  void update_fill_color (QColor &color);
};

#endif // TOOLS_WIDGET_BUILDER_H
