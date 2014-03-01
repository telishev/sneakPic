#include "gui/create_gui_widget.h"
#include "gui/gui_widget_view.h"

#include <QDoubleSpinBox>
#include <QComboBox>

#include "gui/connection.h"

class QDoubleSpinBox_widget : public gui_widget
{
  QDoubleSpinBox *m_spinbox;
public:
  QDoubleSpinBox_widget (QDoubleSpinBox *spinbox) : m_spinbox (spinbox)
  {
    CONNECT (m_spinbox, (void (QDoubleSpinBox::*)(double))&QDoubleSpinBox::valueChanged, this, &gui_widget::data_changed);
  }

  virtual void set_value (QVariant value) override {  m_spinbox->setValue (value.toDouble ()); }
  virtual QVariant value () const override { return m_spinbox->value (); }

};

gui_widget *create_gui_widget (QDoubleSpinBox *widget) { return new QDoubleSpinBox_widget (widget); }

class QComboBox_widget : public gui_widget
{
  QComboBox *m_combobox;
public:
  QComboBox_widget (QComboBox *combobox) : m_combobox (combobox)
  {
    CONNECT (m_combobox, (void (QComboBox::*) (int)) &QComboBox::currentIndexChanged, this, &gui_widget::data_changed);
  }

  virtual void set_value (QVariant value) override
  {
    int index = m_combobox->findData (value);
    m_combobox->setCurrentIndex (index);
  }

  virtual QVariant value () const override
  {
    return m_combobox->itemData (m_combobox->currentIndex ());
  }

};

gui_widget * create_gui_widget (QComboBox *widget) { return new QComboBox_widget (widget); }