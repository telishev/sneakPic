#include "gui/utils/qt_utils.h"
// File to contain some stuff which have to do with Qt, later some parts may be moved to separate files

#include "gui/utils/override_undo_filter.h"

#include <QDoubleSpinBox>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QDialogButtonBox>
#include <QDialog>

#include "gui/connection.h"

QWidget *create_container_widget (QWidget *parent)
{
  QWidget *widget = new QWidget (parent);
  return widget;
}

void finish_with_spacer (QGridLayout *layout)
{
  layout->addItem (new QSpacerItem (0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding), layout->rowCount (), 0);
}

QDialogButtonBox *create_standard_button_box (QDialog *dialog)
{
  auto box = new QDialogButtonBox (QDialogButtonBox::Ok | QDialogButtonBox::Cancel, dialog);
  CONNECT (box, &QDialogButtonBox::accepted, dialog, &QDialog::accept);
  CONNECT (box, &QDialogButtonBox::rejected, dialog, &QDialog::reject);
  return box;
}

QGridLayout *create_common_grid_widget (QWidget *parent)
{
  QWidget *widget = create_container_widget (parent);
  QGridLayout *layout = new QGridLayout (widget);
  layout->setContentsMargins (6, 6, 6, 6);
  layout->setVerticalSpacing (5);
  layout->setHorizontalSpacing (10);
  return layout;
}

QHBoxLayout *create_common_hbox_widget (QWidget *parent)
{
  QWidget *widget = create_container_widget (parent);
  QHBoxLayout *layout = new QHBoxLayout (widget);
  layout->setContentsMargins (6, 6, 6, 6);
  layout->setSpacing (10);
  return layout;
}

QVBoxLayout *create_common_vbox_widget (QWidget *parent)
{
  QWidget *widget = create_container_widget (parent);
  QVBoxLayout *layout = new QVBoxLayout (widget);
  layout->setContentsMargins (6, 6, 6, 6);
  layout->setSpacing (5);
  return layout;
}

QHBoxLayout *create_common_hbox_layout (QWidget *widget)
{
  QHBoxLayout *layout = new QHBoxLayout (widget);
  layout->setContentsMargins (6, 6, 6, 6);
  layout->setSpacing (5);
  return layout;
}

QVBoxLayout *create_common_vbox_layout (QWidget *widget)
{
  QVBoxLayout *layout = new QVBoxLayout (widget);
  layout->setContentsMargins (6, 6, 6, 6);
  layout->setSpacing (5);
  return layout;
}

QGridLayout *create_intermediate_grid_layout (QWidget *parent)
{
  QWidget *widget = create_container_widget (parent);
  if (parent) // if parent is nullptr then layout will be placed somewhere afterwards
    parent->layout ()->addWidget (widget);
  QGridLayout *layout = new QGridLayout (widget);
  layout->setVerticalSpacing (5);
  layout->setHorizontalSpacing (10);
  layout->setContentsMargins (0, 0, 0, 0);
  return layout;
}

QHBoxLayout *create_intermediate_hbox_layout (QWidget *parent)
{
  QWidget *widget = create_container_widget (parent);
  if (parent) // if parent is nullptr then layout will be placed somewhere afterwards
    parent->layout ()->addWidget (widget);
  QHBoxLayout *layout = new QHBoxLayout (widget);
  layout->setSpacing (10);
  layout->setContentsMargins (0, 0, 0, 0);
  return layout;
}

QHBoxLayout *create_intermediate_hbox_layout (QLayout *parent_layout)
{
  return create_intermediate_hbox_layout (parent_layout->parentWidget ());
}

QVBoxLayout *create_intermediate_vbox_layout (QWidget *parent)
{
  QWidget *widget = create_container_widget (parent);
  if (parent) // if parent is nullptr then layout will be placed somewhere afterwards
    parent->layout ()->addWidget (widget);
  QVBoxLayout *layout = new QVBoxLayout (widget);
  layout->setSpacing (5);
  layout->setContentsMargins (0, 0, 0, 0);
  return layout;
}

QVBoxLayout *create_intermediate_vbox_layout (QLayout *parent_layout)
{
  return create_intermediate_vbox_layout (parent_layout->parentWidget ());
}

QDoubleSpinBox *create_double_spinbox (QWidget *parent, double maximum, int precision, double single_step, double minimum)
{
  QDoubleSpinBox *spinbox = new QDoubleSpinBox (parent);
  spinbox->setMaximum (maximum);
  spinbox->setMinimum (minimum);
  spinbox->setDecimals (precision);
  spinbox->setSingleStep (single_step);
  spinbox->installEventFilter (new override_undo_filter (spinbox));
  spinbox->setKeyboardTracking (false);
  return spinbox;
};
