#ifndef CREATE_GUI_WIDGET_H
#define CREATE_GUI_WIDGET_H

class gui_widget;
class QDoubleSpinBox;
class QComboBox;

class gui_widget : public QObject
{
  Q_OBJECT

public:
  virtual ~gui_widget () {}

  virtual void set_value (QVariant value) = 0;
  virtual QVariant value () const = 0;

signals:
  void data_changed ();
};


gui_widget *create_gui_widget (QDoubleSpinBox *widget);
gui_widget *create_gui_widget (QComboBox *widget);

#endif // CREATE_GUI_WIDGET_H
