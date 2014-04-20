#ifndef TOOL_PROPERTIES_WIDGET_H
#define TOOL_PROPERTIES_WIDGET_H
class dock_widget_builder;

class tool_properties_widget
{
  QWidget *m_widget;

public:
  tool_properties_widget (dock_widget_builder *dock_widget_builder_arg);
  ~tool_properties_widget ();
};
#endif // TOOL_PROPERTIES_WIDGET_H

