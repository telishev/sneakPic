#include "gui/tool_properties_widget.h"
#include "gui/dock_widget_builder.h"

#include <QWidget>

tool_properties_widget::tool_properties_widget (dock_widget_builder *dock_widget_builder_arg)
{
  m_widget = new QWidget;
  m_widget->setObjectName ("Tool Properties");
  dock_widget_builder_arg->add_widget (m_widget, Qt::TopDockWidgetArea, visibility_state::visible);
}

tool_properties_widget::~tool_properties_widget ()
{
}
