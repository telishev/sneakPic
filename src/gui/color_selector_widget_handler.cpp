#include "gui/color_selector_widget_handler.h"

#include "gui/color_selectors/color_linear_selector.h"
#include "gui/color_selectors/color_rectangular_selector.h"
#include "gui/color_selectors/color_spinbox.h"
#include "gui/dock_widget_builder.h"
#include "gui/connection.h"
#include "gui/gui_widget_view.h"

#include "gui/utils/qt_utils.h"

#include <QLabel>
#include <QLayout>
#include <QTabWidget>
#include <QWidget>
#include "gui_model.h"

color_selector_widget_handler::color_selector_widget_handler (gui_model *model)
{
  m_model = model;
  m_view.reset (new gui_widget_view (m_model));
  m_color_selector_layout = qt_utils::create_common_vbox_layout (this);
  setSizePolicy (QSizePolicy::Preferred, QSizePolicy::Maximum);
  m_tab_widget = new QTabWidget;
  m_color_selector_layout->addWidget (m_tab_widget);

  add_color_selectors ();
  setObjectName ("Color");
}


color_selector_widget_handler::~color_selector_widget_handler ()
{

}

static inline QString get_label_string_by_type (color_single_selector_type type)
{
  switch (type)
    {
    case color_single_selector_type::HSL_HUE:
    case color_single_selector_type::HSV_HUE:
      return "H";
    case color_single_selector_type::HSL_LIGHTNESS:
      return "L";
    case color_single_selector_type::HSL_SATURATION:
    case color_single_selector_type::HSV_SATURATION:
      return "S";
    case color_single_selector_type::HSV_VALUE:
      return "V";
    case color_single_selector_type::RGB_RED:
      return "R";
    case color_single_selector_type::RGB_GREEN:
      return "G";
    case color_single_selector_type::RGB_BLUE:
      return "B";
    case color_single_selector_type::RGB_ALPHA:
    case color_single_selector_type::HSL_ALPHA:
    case color_single_selector_type::HSV_ALPHA:
    case color_single_selector_type::CMYK_ALPHA:
      return "A";
    case color_single_selector_type::CMYK_CYAN:
      return "C";
    case color_single_selector_type::CMYK_MAGENTA:
      return "M";
    case color_single_selector_type::CMYK_YELLOW:
      return "Y";
    case color_single_selector_type::CMYK_BLACK:
      return "K";
    }
  return "";
}


class color_selector_gui_widget : public gui_widget
{
  color_selector *m_selector;
public:
  color_selector_gui_widget (color_selector *selector, bool momentary_change) : m_selector (selector)
  {
    if (momentary_change)
      CONNECT (m_selector, &color_selector::color_changed_momentarily, this, &gui_widget::data_changed);
    else
      CONNECT (m_selector, &color_selector::color_changing_finished, this, &gui_widget::data_changed);
  }

  virtual void set_value (QVariant value) override {  m_selector->set_color (value.value<QColor> ()); }
  virtual QVariant value () const override { return QVariant::fromValue (m_selector->color ()); }

};

// Adds scroller with label, scroller itself and combobox
void color_selector_widget_handler::add_typical_scroller_widget (QGridLayout *grid_layout, color_single_selector_type type)
{
  QString label_string = get_label_string_by_type (type);
  int row_pos = grid_layout->rowCount ();
  grid_layout->addWidget (new QLabel (label_string, grid_layout->parentWidget ()), row_pos, 0);
  color_linear_selector *color_linear_selector_widget = new color_linear_selector (grid_layout->parentWidget (), Qt::Horizontal, type);
  register_color_selector (color_linear_selector_widget);
  grid_layout->addWidget (color_linear_selector_widget, row_pos, 1);
  color_spinbox *color_spinbox_widget = new color_spinbox (grid_layout->parentWidget (), type);
  register_color_selector (color_spinbox_widget);
  grid_layout->addWidget (color_spinbox_widget, row_pos, 2);
}

void color_selector_widget_handler::add_color_selectors ()
{
  // create everything
  create_rgb_tab();
  create_hsl_tab();
  create_hsv_tab();
  create_cmyk_widget();
}

void color_selector_widget_handler::create_rgb_tab ()
{
  QGridLayout *rgb_layout = qt_utils::create_common_grid_widget (m_tab_widget);
  add_typical_scroller_widget (rgb_layout, color_single_selector_type::RGB_RED);
  add_typical_scroller_widget (rgb_layout, color_single_selector_type::RGB_GREEN);
  add_typical_scroller_widget (rgb_layout, color_single_selector_type::RGB_BLUE);
  add_typical_scroller_widget (rgb_layout, color_single_selector_type::RGB_ALPHA);
  qt_utils::finish_with_spacer (rgb_layout);
  m_tab_widget->addTab (rgb_layout->parentWidget (), QIcon (), "RGB");
}

void color_selector_widget_handler::create_hsl_tab ()
{
  QGridLayout *hsl_layout = qt_utils::create_common_grid_widget (m_tab_widget);
  add_typical_scroller_widget (hsl_layout, color_single_selector_type::HSL_HUE);
  add_typical_scroller_widget (hsl_layout, color_single_selector_type::HSL_SATURATION);
  add_typical_scroller_widget (hsl_layout, color_single_selector_type::HSL_LIGHTNESS);
  add_typical_scroller_widget (hsl_layout, color_single_selector_type::HSL_ALPHA);
  qt_utils::finish_with_spacer (hsl_layout);
  m_tab_widget->addTab (hsl_layout->parentWidget (), QIcon (), "HSL");
}

void color_selector_widget_handler::create_hsv_tab ()
{
  QVBoxLayout *hsv_tab_layout = qt_utils::create_common_vbox_widget (m_tab_widget);
  QHBoxLayout *hsv_layout = qt_utils::create_intermediate_hbox_layout (hsv_tab_layout->parentWidget ());
  color_rectangular_selector *color_rect_widget = new color_rectangular_selector (hsv_layout->parentWidget (), color_single_selector_type::HSV_SATURATION, color_single_selector_type::HSV_VALUE);
  register_color_selector (color_rect_widget);
  hsv_layout->addWidget (color_rect_widget);
  color_linear_selector *hue_widget = new color_linear_selector (hsv_layout->parentWidget (), Qt::Vertical, color_single_selector_type::HSV_HUE);
  hue_widget->setSizePolicy (QSizePolicy::Fixed, QSizePolicy::Fixed);
  register_color_selector (hue_widget);
  hsv_layout->addWidget (hue_widget);

  hsv_layout->addStretch ();
  QGridLayout *alpha_layout = qt_utils::create_common_grid_widget (hsv_tab_layout->parentWidget ());
  add_typical_scroller_widget (alpha_layout, color_single_selector_type::HSV_ALPHA);
  hsv_tab_layout->addWidget (alpha_layout->parentWidget ());
  hsv_tab_layout->addStretch ();
  m_tab_widget->addTab (hsv_tab_layout->parentWidget (), QIcon (), "HSV");
}

void color_selector_widget_handler::create_cmyk_widget ()
{
  QGridLayout *cmyk_layout = qt_utils::create_common_grid_widget (m_tab_widget);
  cmyk_layout->setVerticalSpacing (5);
  cmyk_layout->setHorizontalSpacing (10);
  add_typical_scroller_widget (cmyk_layout, color_single_selector_type::CMYK_CYAN);
  add_typical_scroller_widget (cmyk_layout, color_single_selector_type::CMYK_MAGENTA);
  add_typical_scroller_widget (cmyk_layout, color_single_selector_type::CMYK_YELLOW);
  add_typical_scroller_widget (cmyk_layout, color_single_selector_type::CMYK_BLACK);
  add_typical_scroller_widget (cmyk_layout, color_single_selector_type::CMYK_ALPHA);
  qt_utils::finish_with_spacer (cmyk_layout);
  m_tab_widget->addTab (cmyk_layout->parentWidget (), QIcon (), "CMYK");
}

void color_selector_widget_handler::register_color_selector (color_selector *selector)
{
  m_view->add_gui_widget (gui_model_role_t::CURRENT_COLOR_TEMP, new color_selector_gui_widget (selector, true));
  m_view->add_gui_widget (gui_model_role_t::CURRENT_COLOR, new color_selector_gui_widget (selector, false));
}
