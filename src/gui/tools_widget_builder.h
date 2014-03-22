#ifndef TOOLS_WIDGET_BUILDER_H
#define TOOLS_WIDGET_BUILDER_H

#include <QObject>

class QDockWidget;
class QMainWindow;
class QToolBar;
class QWidget;

class FlowLayout;

enum class gui_action_id;
enum class style_controller_role_t;

class color_indicator;
class gui_actions;
class dock_widget_builder;
class fill_stroke_widget;
class item_paint_style;
template<typename T>
class gui_model;

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
  fill_stroke_widget *m_fill_stroke;

public:
  tools_widget_builder (gui_actions *actions, dock_widget_builder *dock_widget_builder_arg, gui_model<style_controller_role_t> *model);
  ~tools_widget_builder ();

  void update ();
private:
  void fill_tool_bar ();
  void init_toolbar_and_layout();
  void add_action (gui_action_id id);
  void add_color_indicators ();
};

#endif // TOOLS_WIDGET_BUILDER_H
