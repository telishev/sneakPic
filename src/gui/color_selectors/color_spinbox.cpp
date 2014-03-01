#include "gui/color_selectors/color_spinbox.h"

#include "gui/connection.h"

#include <QHBoxLayout>
#include <QSpinBox>

color_spinbox::color_spinbox (QWidget *parent, color_single_selector_type type) : color_selector (parent)
{
  m_layout = new QHBoxLayout ();
  m_type = type;
  this->setLayout (m_layout);
  m_spin_box = new QSpinBox ();
  m_spin_box->setMaximum (get_param_maximum_by_type (m_type));
  m_spin_box->setMinimum (0);
  m_spin_box->setValue (get_param_value_by_type (m_type));
  m_spin_box->setSingleStep (1);
  m_layout->addWidget (m_spin_box);
  m_layout->setContentsMargins (0, 0, 0, 0);
  m_value_changed_connection = CONNECT (m_spin_box, (void (QSpinBox::*) (int)) &QSpinBox::valueChanged, this, &color_spinbox::spinbox_value_changed);
}

color_spinbox::~color_spinbox ()
{
}

void color_spinbox::set_color (QColor color)
{
  TEMPORARY_DISCONNECT (m_value_changed_connection);
  m_color = color;
  m_spin_box->setValue (get_param_value_by_type (m_type));
}

void color_spinbox::spinbox_value_changed (int new_value)
{
  change_param_by_type (m_color, new_value, m_type);
  emit color_changing_finished (m_color);
  emit color_changed_momentarily (m_color);
}



