#include "qt_utils.h"
// File to contain some stuff which have to do with Qt, later some parts may be moved to separate files

#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>

QWidget *create_inner_widget (QWidget *parent)
{
  QWidget *widget = new QWidget (parent);
  widget->setContentsMargins (0, 0, 0, 0);
  return widget;
}

void finish_with_spacer (QVBoxLayout *layout)
{
  layout->addItem (new QSpacerItem (0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
}

void finish_with_spacer (QHBoxLayout *layout)
{
  layout->addItem (new QSpacerItem (0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
}

void finish_with_spacer (QGridLayout *layout)
{
  layout->addItem (new QSpacerItem (0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding), layout->rowCount (), 0);
}

QGridLayout *create_common_grid_layout (QWidget *parent)
{
  QWidget *widget = new QWidget (parent);
  QGridLayout *layout = new QGridLayout (widget);
  layout->setVerticalSpacing (5);
  layout->setHorizontalSpacing (10);
  return layout;
}

QHBoxLayout *create_common_hbox_layout (QWidget *parent)
{
  QWidget *widget = new QWidget (parent);
  QHBoxLayout *layout = new QHBoxLayout (widget);
  return layout;
}

QVBoxLayout *create_common_vbox_layout (QWidget *parent)
{
  QWidget *widget = new QWidget (parent);
  QVBoxLayout *layout = new QVBoxLayout (widget);
  return layout;
}

QGridLayout *create_inner_grid_layout (QWidget *parent)
{
  QWidget *widget = new QWidget (parent);
  parent->layout ()->addWidget (widget);
  QGridLayout *layout = new QGridLayout (widget);
  layout->setVerticalSpacing (5);
  layout->setHorizontalSpacing (10);
  return layout;
}

QHBoxLayout *create_inner_hbox_layout (QWidget *parent)
{
  QWidget *widget = create_inner_widget (parent);
  parent->layout ()->addWidget (widget);
  QHBoxLayout *layout = new QHBoxLayout (widget);
  return layout;
}

QVBoxLayout *create_inner_vbox_layout (QWidget *parent)
{
  QWidget *widget = create_inner_widget (parent);
  parent->layout ()->addWidget (widget);
  QVBoxLayout *layout = new QVBoxLayout (widget);
  return layout;
}

