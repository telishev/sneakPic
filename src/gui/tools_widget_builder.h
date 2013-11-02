#ifndef TOOLS_WIDGET_BUILDER_H
#define TOOLS_WIDGET_BUILDER_H

class QDockWidget;
class QMainWindow;
class QToolBar;
class QWidget;

class FlowLayout;

enum class gui_action_id;

class gui_actions;
class dock_widget_builder;

class tools_widget_builder
{
  gui_actions *m_actions;
  QMainWindow *m_main_window;
  QWidget *m_tool_bar;
  FlowLayout *m_layout;
  int m_count;
  dock_widget_builder *m_dock_widget_builder;

public:
  tools_widget_builder (gui_actions *actions, dock_widget_builder *dock_widget_builder_arg);
  ~tools_widget_builder ();

  void update ();

private:
  void fill_tool_bar ();
  void init_toolbar_and_layout();
  void add_action (gui_action_id id);
};

#endif // TOOLS_WIDGET_BUILDER_H
