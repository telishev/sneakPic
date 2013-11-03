#include "gui/dock_widget_builder.h"

#include <QDockWidget>
#include <QMainWindow>
#include <QWidget>

dock_widget_builder::dock_widget_builder (QMainWindow *main_window)
{
  m_main_window = main_window;
}

void dock_widget_builder::add_widget (QWidget *widget, Qt::DockWidgetArea default_pos, visibility_state default_visibility, Qt::DockWidgetAreas allowed_areas)
{
  QDockWidget *dock_widget = new QDockWidget (widget->objectName ());
  widget->setParent (dock_widget);
  dock_widget->setObjectName (widget->objectName ());
  dock_widget->setFeatures (QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
  dock_widget->setWidget (widget);
  dock_widget->setAllowedAreas (allowed_areas);
  m_main_window->addDockWidget (default_pos, dock_widget);
  dock_widget->setVisible (default_visibility == visibility_state::visible);
}

