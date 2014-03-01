#include "color_selector.h"

#include <QHBoxLayout>
#include <QMouseEvent>
#include <QSpinBox>
#include <QPainter>

#include "common/memory_deallocation.h"

#include "gui/mouse_filter.h"

#include <stdlib.h>
#include "gui/paint_helper.h"

bool color_selector::is_selector_type_alpha (color_single_selector_type type)
{
  switch (type)
    {
    case color_single_selector_type::RGB_ALPHA:
    case color_single_selector_type::HSL_ALPHA:
    case color_single_selector_type::HSV_ALPHA:
    case color_single_selector_type::CMYK_ALPHA:
      return true;
    default: return false;
    }
}

void color_selector::draw_checkerboard (QPainter &painter)
{
  paint_helper ().draw_checkerboard (painter, width (), height ());
}

QColor color_selector::change_param_by_type (QColor color, int value, color_single_selector_type type)
{
  int h = 0, s, v, l, r, g, b, a, c, m, y, k;
  switch (type)
    {
    case color_single_selector_type::HSV_SATURATION:
    case color_single_selector_type::HSV_HUE:
    case color_single_selector_type::HSV_VALUE:
    case color_single_selector_type::HSV_ALPHA:
      color.getHsv (&h, &s, &v, &a);
      break;
    case color_single_selector_type::HSL_HUE:
    case color_single_selector_type::HSL_SATURATION:
    case color_single_selector_type::HSL_LIGHTNESS:
    case color_single_selector_type::HSL_ALPHA:
      color.getHsl (&h, &s, &l, &a);
      break;

    case color_single_selector_type::RGB_RED:
    case color_single_selector_type::RGB_GREEN:
    case color_single_selector_type::RGB_BLUE:
    case color_single_selector_type::RGB_ALPHA:
      color.getRgb (&r, &g, &b, &a);
      break;

    case color_single_selector_type::CMYK_CYAN:
    case color_single_selector_type::CMYK_MAGENTA:
    case color_single_selector_type::CMYK_YELLOW:
    case color_single_selector_type::CMYK_BLACK:
    case color_single_selector_type::CMYK_ALPHA:
      color.getCmyk (&c, &m, &y, &k, &a);
      break;
    }

  if (value < 0)
    value = 0;

  if (value > get_param_maximum_by_type (type))
    value = get_param_maximum_by_type (type);

  if (h < 0)
    h = 0;

  switch (type)
    {
    case color_single_selector_type::HSL_HUE:
      color.setHsl (value, s, l, a);
      break;
    case color_single_selector_type::HSV_HUE:
      color.setHsv (value, s, v, a);
      break;
    case color_single_selector_type::HSL_LIGHTNESS:
      color.setHsl (h, s, value, a);
      break;
    case color_single_selector_type::HSL_SATURATION:
      color.setHsl (h, value, l, a);
      break;
    case color_single_selector_type::HSV_SATURATION:
      color.setHsv (h, value, v, a);
      break;
    case color_single_selector_type::HSV_VALUE:
      color.setHsv (h, s, value, a);
      break;
    case color_single_selector_type::RGB_RED:
      color.setRgb (value, g, b, a);
      break;
    case color_single_selector_type::RGB_GREEN:
      color.setRgb (r, value, b, a);
      break;
    case color_single_selector_type::RGB_BLUE:
      color.setRgb (r, g, value, a);
      break;
    case color_single_selector_type::RGB_ALPHA:
      color.setRgb (r, g, b, value);
      break;
    case color_single_selector_type::CMYK_CYAN:
      color.setCmyk (value, m, y, k, a);
      break;
    case color_single_selector_type::CMYK_MAGENTA:
      color.setCmyk (c, value, y, k, a);
      break;
    case color_single_selector_type::CMYK_YELLOW:
      color.setCmyk (c, m, value, k, a);
      break;
    case color_single_selector_type::CMYK_BLACK:
      color.setCmyk (c, m, y, value, a);
      break;
    case color_single_selector_type::HSL_ALPHA:
      color.setHsl (h, s, l, value);
      break;
    case color_single_selector_type::HSV_ALPHA:
      color.setHsv (h, s, v, value);
      break;
    case color_single_selector_type::CMYK_ALPHA:
      color.setCmyk (c, m, y, k, value);
      break;
    }

  return color;
}

int color_selector::get_param_value_by_type (color_single_selector_type type)
{
  int h = 0, s, v, l, r, g, b, a, c, m, y, k;

  switch (type)
    {
    case color_single_selector_type::HSV_SATURATION:
    case color_single_selector_type::HSV_HUE:
    case color_single_selector_type::HSV_VALUE:
    case color_single_selector_type::HSV_ALPHA:
      m_color.getHsv (&h, &s, &v, &a);
      break;
    case color_single_selector_type::HSL_HUE:
    case color_single_selector_type::HSL_SATURATION:
    case color_single_selector_type::HSL_LIGHTNESS:
    case color_single_selector_type::HSL_ALPHA:
      m_color.getHsl (&h, &s, &l, &a);
      break;

    case color_single_selector_type::RGB_RED:
    case color_single_selector_type::RGB_GREEN:
    case color_single_selector_type::RGB_BLUE:
    case color_single_selector_type::RGB_ALPHA:
      m_color.getRgb (&r, &g, &b, &a);
      break;

    case color_single_selector_type::CMYK_CYAN:
    case color_single_selector_type::CMYK_MAGENTA:
    case color_single_selector_type::CMYK_YELLOW:
    case color_single_selector_type::CMYK_BLACK:
    case color_single_selector_type::CMYK_ALPHA:
      m_color.getCmyk (&c, &m, &y, &k, &a);
      break;
    }

  if (h < 0)
    h = 0;

  switch (type)
    {
    case color_single_selector_type::HSL_HUE:
    case color_single_selector_type::HSV_HUE:
      return h;
    case color_single_selector_type::HSL_LIGHTNESS:
      return l;
    case color_single_selector_type::HSL_SATURATION:
    case color_single_selector_type::HSV_SATURATION:
      return s;
    case color_single_selector_type::RGB_RED:
      return r;
    case color_single_selector_type::RGB_GREEN:
      return g;
    case color_single_selector_type::RGB_BLUE:
      return b;
    case color_single_selector_type::RGB_ALPHA:
    case color_single_selector_type::HSL_ALPHA:
    case color_single_selector_type::HSV_ALPHA:
    case color_single_selector_type::CMYK_ALPHA:
      return a;
    case color_single_selector_type::CMYK_CYAN:
      return c;
    case color_single_selector_type::CMYK_MAGENTA:
      return m;
    case color_single_selector_type::CMYK_YELLOW:
      return y;
    case color_single_selector_type::CMYK_BLACK:
      return k;
    case color_single_selector_type::HSV_VALUE:
      return v;
    }
  return 0;
}

void color_selector::draw_border (QPainter &painter)
{
  paint_helper ().draw_border (painter, width (), height ());
}

QColor color_selector::do_color_preprocessing_by_type (QColor color, color_single_selector_type type)
{
  switch (type)
    {
    case color_single_selector_type::HSL_HUE:
      color.setHsl (0, 255, 128, 255); // In this case we need basically the same palette for every color (rainbow)
      break;
    case color_single_selector_type::HSV_HUE:
      color.setHsv (0, 255, 255, 255);
      break;
    case color_single_selector_type::HSL_LIGHTNESS:
    case color_single_selector_type::HSL_SATURATION:
      return change_param_by_type (color, 255, color_single_selector_type::HSL_ALPHA);
    case color_single_selector_type::HSV_SATURATION:
    case color_single_selector_type::HSV_VALUE:
      return change_param_by_type (color, 255, color_single_selector_type::HSV_ALPHA);
    case color_single_selector_type::CMYK_CYAN:
    case color_single_selector_type::CMYK_MAGENTA:
    case color_single_selector_type::CMYK_YELLOW:
    case color_single_selector_type::CMYK_BLACK:
      return change_param_by_type (color, 255, color_single_selector_type::CMYK_ALPHA);
    case color_single_selector_type::RGB_RED:
    case color_single_selector_type::RGB_GREEN:
    case color_single_selector_type::RGB_BLUE:
      return change_param_by_type (color, 255, color_single_selector_type::RGB_ALPHA);
    case color_single_selector_type::RGB_ALPHA:
    case color_single_selector_type::HSL_ALPHA:
    case color_single_selector_type::HSV_ALPHA:
    case color_single_selector_type::CMYK_ALPHA:
      break;
    }

  return color;
}

QColor color_selector::do_color_preprocessing_by_two_types (QColor color, color_single_selector_type first_type, color_single_selector_type second_type)
{
  if (!is_selector_type_alpha (first_type) && !is_selector_type_alpha (second_type))
    color.setAlpha (255);

  return color;
}

int color_selector::get_needed_number_of_points_for_gradient_by_type (color_single_selector_type type)
{
  switch (type)
    {
    case color_single_selector_type::HSL_HUE:
    case color_single_selector_type::HSV_HUE:
      return 7;
      break;
    case color_single_selector_type::HSL_LIGHTNESS:
      return 3;
      break;
    case color_single_selector_type::HSL_SATURATION:
    case color_single_selector_type::HSV_SATURATION:
    case color_single_selector_type::HSV_VALUE:
    case color_single_selector_type::RGB_RED:
    case color_single_selector_type::RGB_GREEN:
    case color_single_selector_type::RGB_BLUE:
    case color_single_selector_type::RGB_ALPHA:
    case color_single_selector_type::HSL_ALPHA:
    case color_single_selector_type::HSV_ALPHA:
    case color_single_selector_type::CMYK_ALPHA:
    case color_single_selector_type::CMYK_CYAN:
    case color_single_selector_type::CMYK_MAGENTA:
    case color_single_selector_type::CMYK_YELLOW:
    case color_single_selector_type::CMYK_BLACK:
      return 2;
    }
  return 0;
}

int color_selector::get_param_maximum_by_type (color_single_selector_type type)
{
  switch (type)
    {
    case color_single_selector_type::HSL_HUE:
    case color_single_selector_type::HSV_HUE:
      return 359;
    case color_single_selector_type::HSL_LIGHTNESS:
    case color_single_selector_type::HSL_SATURATION:
    case color_single_selector_type::HSV_SATURATION:
    case color_single_selector_type::RGB_RED:
    case color_single_selector_type::RGB_BLUE:
    case color_single_selector_type::RGB_GREEN:
    case color_single_selector_type::RGB_ALPHA:
    case color_single_selector_type::HSL_ALPHA:
    case color_single_selector_type::HSV_ALPHA:
    case color_single_selector_type::CMYK_ALPHA:
    case color_single_selector_type::CMYK_CYAN:
    case color_single_selector_type::CMYK_MAGENTA:
    case color_single_selector_type::CMYK_BLACK:
    case color_single_selector_type::CMYK_YELLOW:
    case color_single_selector_type::HSV_VALUE:
      return 255;
    }
  return 0;
}

void color_selector::set_color (QColor color)
{
  m_color = color;
  update ();
}

color_selector::color_selector (QWidget *parent) : QWidget (parent)
{
}

