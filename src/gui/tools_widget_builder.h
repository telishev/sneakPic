#ifndef TOOLS_WIDGET_BUILDER_H
#define TOOLS_WIDGET_BUILDER_H

class gui_actions;
class QDockWidget;
class QGridLayout;
class QMainWindow;
class QToolBar;
class QWidget;

enum class gui_action_id;

class tools_widget_builder
{
  gui_actions *m_actions;
  QMainWindow *m_main_window;
  QDockWidget *m_tool_bar_window;
  QWidget *m_tool_bar;
  QGridLayout *m_layout;
  int m_count;
public:
  tools_widget_builder (gui_actions *actions, QMainWindow *main_window);
  ~tools_widget_builder ();

  void update ();

private:
  void fill_tool_bar ();
  void init_toolbar_and_layout();
  void add_action (gui_action_id id);
};

#endif // TOOLS_WIDGET_BUILDER_H
