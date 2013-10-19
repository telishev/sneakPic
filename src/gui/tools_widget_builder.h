#ifndef TOOLS_WIDGET_BUILDER_H
#define TOOLS_WIDGET_BUILDER_H

class gui_actions;
class QMainWindow;
class QToolBar;

enum class gui_action_id;

class tools_widget_builder
{
  gui_actions *m_actions;
  QMainWindow *m_main_window;
  QToolBar *m_tool_bar;
public:
  tools_widget_builder (gui_actions *actions, QMainWindow *main_window);
  ~tools_widget_builder ();

  void update ();
private:
  void fill_tool_bar ();
  void add_action (gui_action_id id);
};

#endif // TOOLS_WIDGET_BUILDER_H
