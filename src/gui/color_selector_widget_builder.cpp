#include "gui/color_selector_widget_builder.h"

#include "gui/color_selectors.h"
#include "gui/dock_widget_builder.h"
#include "gui/connection.h"

#include <QLabel>
#include <QLayout>
#include <QTabWidget>
#include <QWidget>

color_selector_widget_builder::color_selector_widget_builder (dock_widget_builder *dock_widget_builder_arg, QColor *color)
{
  m_dock_widget_builder = dock_widget_builder_arg;
  m_color_selector_widget = new QWidget (0);
  m_layout = new QVBoxLayout ();
  m_color_selector_widget->setLayout (m_layout);
  m_tab_widget = new QTabWidget (m_color_selector_widget);
  m_layout->addWidget (m_tab_widget);
  m_color = color;

  add_color_selectors ();
  m_color_selector_widget->setObjectName ("Color");

  m_dock_widget_builder->add_widget (m_color_selector_widget, Qt::RightDockWidgetArea, visibility_state::visible);
}

void color_selector_widget_builder::update_colors ()
{
  foreach (color_selector * widget, m_color_widgets)
  {
    widget->color_changed_externally ();
  }
  emit color_changed ();
}

static inline QString get_label_string_by_type (color_single_selector_type type)
{
  switch (type)
    {
    case color_single_selector_type::HUE_HSL:
    case color_single_selector_type::HUE_HSV:
      return "H";
    case color_single_selector_type::LIGHTNESS:
      return "L";
    case color_single_selector_type::SATURATION_HSL:
    case color_single_selector_type::SATURATION_HSV:
      return "S";
    case color_single_selector_type::VALUE:
      return "V";
    case color_single_selector_type::RED:
      return "R";
    case color_single_selector_type::GREEN:
      return "G";
    case color_single_selector_type::BLUE:
      return "B";
    case color_single_selector_type::ALPHA:
      return "A";
    case color_single_selector_type::CYAN:
      return "C";
    case color_single_selector_type::MAGENTA:
      return "M";
    case color_single_selector_type::YELLOW:
      return "Y";
    case color_single_selector_type::BLACK:
      return "K";
    }
  return "";
}

// Adds scroller with label, scroller itself and combobox
void color_selector_widget_builder::add_typical_scroller_widget (QGridLayout *grid_layout, color_single_selector_type type)
{
  QString label_string = get_label_string_by_type (type);
  int row_pos = grid_layout->rowCount ();
  grid_layout->addWidget (new QLabel (label_string, grid_layout->parentWidget ()), row_pos, 0);
  color_linear_selector *color_linear_selector_widget = new color_linear_selector (grid_layout->parentWidget (), Qt::Horizontal, type, m_color);
  m_color_widgets << color_linear_selector_widget;
  grid_layout->addWidget (color_linear_selector_widget, row_pos, 1);
  color_spinbox *color_spinbox_widget = new color_spinbox (grid_layout->parentWidget (), type, m_color);
  m_color_widgets << color_spinbox_widget;
  grid_layout->addWidget (color_spinbox_widget, row_pos, 2);
}

void color_selector_widget_builder::add_color_selectors ()
{
  QWidget *rgb_widget = new QWidget (m_tab_widget);
  QGridLayout *rgb_layout = new QGridLayout (rgb_widget);
  rgb_layout->setVerticalSpacing (5);
  rgb_layout->setHorizontalSpacing (10);
  add_typical_scroller_widget (rgb_layout, color_single_selector_type::RED);
  add_typical_scroller_widget (rgb_layout, color_single_selector_type::GREEN);
  add_typical_scroller_widget (rgb_layout, color_single_selector_type::BLUE);
  add_typical_scroller_widget (rgb_layout, color_single_selector_type::ALPHA);
  rgb_layout->addItem (new QSpacerItem (0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding), rgb_layout->rowCount (), 0);
  m_tab_widget->addTab (rgb_widget, QIcon (), "RGB");



  QWidget *hsl_widget = new QWidget (m_tab_widget);
  QGridLayout *hsl_layout = new QGridLayout (hsl_widget);
  hsl_layout->setVerticalSpacing (5);
  hsl_layout->setHorizontalSpacing (10);
  add_typical_scroller_widget (hsl_layout, color_single_selector_type::HUE_HSL);
  add_typical_scroller_widget (hsl_layout, color_single_selector_type::SATURATION_HSL);
  add_typical_scroller_widget (hsl_layout, color_single_selector_type::LIGHTNESS);
  add_typical_scroller_widget (hsl_layout, color_single_selector_type::ALPHA);
  hsl_layout->addItem (new QSpacerItem (0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding), hsl_layout->rowCount (), 0);
  m_tab_widget->addTab (hsl_widget, QIcon (), "HSL");

  QWidget *hsv_tab_widget = new QWidget (m_tab_widget);
  QVBoxLayout *hsv_tab_layout = new QVBoxLayout (hsv_tab_widget);
  QWidget *hsv_widget = new QWidget (hsv_tab_widget);
  QHBoxLayout *hsv_layout = new QHBoxLayout (hsv_widget);
  color_rectangular_selector *color_rect_widget = new color_rectangular_selector (rgb_widget, color_single_selector_type::SATURATION_HSV, color_single_selector_type::VALUE, m_color);
  m_color_widgets << color_rect_widget;
  hsv_layout->addWidget (color_rect_widget);
  color_linear_selector *hue_widget = new color_linear_selector (hsv_widget, Qt::Vertical, color_single_selector_type::HUE_HSV, m_color);
  m_color_widgets << hue_widget;
  hsv_layout->addWidget (hue_widget);
  hsv_layout->addItem (new QSpacerItem (0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
  hsv_tab_layout->addWidget (hsv_widget);
  hsv_tab_layout->addItem (new QSpacerItem (0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
  m_tab_widget->addTab (hsv_tab_widget, QIcon (), "HSV");


  QWidget *cmyk_widget = new QWidget (m_tab_widget);
  QGridLayout *cmyk_layout = new QGridLayout (cmyk_widget);
  cmyk_layout->setVerticalSpacing (5);
  cmyk_layout->setHorizontalSpacing (10);
  add_typical_scroller_widget (cmyk_layout, color_single_selector_type::CYAN);
  add_typical_scroller_widget (cmyk_layout, color_single_selector_type::MAGENTA);
  add_typical_scroller_widget (cmyk_layout, color_single_selector_type::YELLOW);
  add_typical_scroller_widget (cmyk_layout, color_single_selector_type::BLACK);
  add_typical_scroller_widget (cmyk_layout, color_single_selector_type::ALPHA);
  cmyk_layout->addItem (new QSpacerItem (0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding), cmyk_layout->rowCount (), 0);
  m_tab_widget->addTab (cmyk_widget, QIcon (), "CMYK");




  foreach (QWidget * widget, m_color_widgets)
  {
    CONNECT (widget, SIGNAL (color_changed_internally ()), this, SLOT (update_colors ()));
  }
}
