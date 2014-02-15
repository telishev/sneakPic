#ifndef COLOR_SPINBOX_H
#define COLOR_SPINBOX_H
#include "gui/color_selectors/color_selector.h"

#include <memory>

class connection;

class color_spinbox : public color_selector
{
  Q_OBJECT
  color_single_selector_type m_type;
  QHBoxLayout *m_layout;
  QSpinBox *m_spin_box;

  unique_ptr<connection> m_value_changed_connection;

public:
  color_spinbox (QWidget *parent, color_single_selector_type type, QColor *color);
  ~color_spinbox ();

protected slots:
  virtual void color_changed_externally();

private slots:
  void spinbox_value_changed (int new_value);
};

#endif // COLOR_SPINBOX_H

