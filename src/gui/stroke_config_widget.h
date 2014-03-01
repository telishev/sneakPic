#ifndef STROKE_CONFIG_WIDGET_H
#define STROKE_CONFIG_WIDGET_H

#include <QWidget>
#include "editor/stroke_config.h"

class QDoubleSpinBox;
class QComboBox;
class QVBoxLayout;
class connection;
class gui_model;
class gui_widget_view;

class stroke_config_widget : public QWidget
{
  unique_ptr<gui_widget_view> m_view;

  QVBoxLayout *m_stroke_style_layout;
  QDoubleSpinBox *m_stroke_width_spinbox;
  QComboBox *m_stroke_linejoin_combobox;
  QDoubleSpinBox *m_stroke_miterlimit_spinbox;
  QComboBox *m_stroke_linecap_combobox;
public:
  stroke_config_widget (gui_model *model, QWidget *parent = 0);
  ~stroke_config_widget ();

private:
  void init_linejoin_controller ();
  void init_miterlimit_controller ();
  void init_stroke_width_controller ();
  void init_linecap_controller ();
};

#endif // STROKE_CONFIG_WIDGET_H
