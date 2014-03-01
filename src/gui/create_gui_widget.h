#ifndef CREATE_GUI_WIDGET_H
#define CREATE_GUI_WIDGET_H

class gui_widget;
class QDoubleSpinBox;
class QComboBox;

gui_widget *create_gui_widget (QDoubleSpinBox *widget);
gui_widget *create_gui_widget (QComboBox *widget);

#endif // CREATE_GUI_WIDGET_H
