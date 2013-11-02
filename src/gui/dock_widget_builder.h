#ifndef DOCK_WIDGET_BUILDER_H
#define DOCK_WIDGET_BUILDER_H

// class to wrap widgets which would be placed and main window and be docable inside it, plus:
// * Their position, geometry and visibility should be preserved
// * Their should be added to corresponded main menu item, where they can be restored

#include <qnamespace.h>

class QMainWindow;
class QWidget;

enum class visibility_state
{
  visible,
  hidden
};

class dock_widget_builder
{
  QMainWindow *m_main_window;
public:
  dock_widget_builder (QMainWindow *main_window);

  void add_widget (QWidget *widget, Qt::DockWidgetArea default_pos, visibility_state default_visibility);
};

#endif // DOCK_WIDGET_BUILDER_H

