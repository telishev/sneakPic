#include "svg/attributes/svg_attribute_preserve_aspect_ratio.h"

#include "svg/items/abstract_svg_item.h"
#include "svg/items/svg_item_type.h"

#include "common/enum_conversion.h"
#include "common/string_utils.h"

#include <QRect>
#include <QString>

svg_attribute_preserve_aspect_ratio::svg_attribute_preserve_aspect_ratio (abstract_svg_item *item)
  : abstract_attribute (item)
{
  m_defer = false;
  m_x_align = perserve_aspect_ratio_align::NONE;
  m_y_align = perserve_aspect_ratio_align::NONE;
  m_behaviour = perserve_aspect_ratio_behaviour::MEET;
}

svg_attribute_preserve_aspect_ratio::~svg_attribute_preserve_aspect_ratio ()
{
}

int enum_values_count (perserve_aspect_ratio_align)
{
  return (int) perserve_aspect_ratio_align::INVALID;
}

const char *enum_to_string (perserve_aspect_ratio_align align)
{
  switch (align)
    {
      case perserve_aspect_ratio_align::MIN: return "Min";
      case perserve_aspect_ratio_align::MID: return "Mid";
      case perserve_aspect_ratio_align::MAX: return "Max";
      case perserve_aspect_ratio_align::NONE: return "";
      case perserve_aspect_ratio_align::INVALID: return "";
    }
  return "";
}


int enum_values_count (perserve_aspect_ratio_behaviour)
{
  return (int) perserve_aspect_ratio_behaviour::INVALID;
}

const char *enum_to_string (perserve_aspect_ratio_behaviour align)
{
  switch (align)
    {
      case perserve_aspect_ratio_behaviour::MEET: return "meet";
      case perserve_aspect_ratio_behaviour::SLICE: return "slice";
      case perserve_aspect_ratio_behaviour::INVALID: return "";
    }
  return "";
}

bool svg_attribute_preserve_aspect_ratio::read (const char *data, bool /*from_css*/)
{
  trim_whitespace_left (data);
  if (starts_with_and_shift (data, "defer"))
    m_defer = true; // this shit works only for linked svgs

  trim_whitespace_left (data);
  if (starts_with (data, "none"))
    return true;

  if (*data != 'x') // consistency check
    return false;
  data++;

  m_x_align = process_string_fixed_length_to_enum <perserve_aspect_ratio_align> (data, 3);
  if (   m_x_align == perserve_aspect_ratio_align::INVALID // consistency check
      || m_x_align == perserve_aspect_ratio_align::NONE)
    return false;

  if (*data != 'Y') // consistency check
    return false;
  data++;

  
  m_y_align = process_string_fixed_length_to_enum <perserve_aspect_ratio_align> (data, 3);
  if (   m_y_align == perserve_aspect_ratio_align::INVALID // consistency check
      || m_y_align == perserve_aspect_ratio_align::NONE)
    return false;

  if (*data == '\0')
    return true;

  if (*data != ' ') // consistency check
    return false;

  trim_whitespace_left (data);
  
  m_behaviour = string_to_enum <perserve_aspect_ratio_behaviour> (data);
  return true;
}

QRectF svg_attribute_preserve_aspect_ratio::get_desired_rect (QRectF &original_rect, QRectF viewport_rect) const
{
  // defer for now ignored, anyway svg in <image> is unsupported
  if (m_x_align == perserve_aspect_ratio_align::NONE && 
      m_y_align == perserve_aspect_ratio_align::NONE
     )
     return viewport_rect;

  double x_ratio = viewport_rect.width () / original_rect.width ();
  double y_ratio = viewport_rect.height () / original_rect.height ();

  if (m_behaviour != perserve_aspect_ratio_behaviour::SLICE)
    {
      if (x_ratio < y_ratio)
        y_ratio = x_ratio;
      else
        x_ratio = y_ratio;
    }
  else
    {
      if (x_ratio < y_ratio)
        x_ratio = y_ratio;
      else
        y_ratio = x_ratio;
    }
  QRectF output_rect;
  output_rect.setWidth (original_rect.width () * x_ratio);
  output_rect.setHeight (original_rect.height () * y_ratio);
  switch (m_x_align)
    {
      case perserve_aspect_ratio_align::MIN:
        output_rect.moveLeft (viewport_rect.x ());
        break;
      case perserve_aspect_ratio_align::MAX:
        output_rect.moveLeft (viewport_rect.x () + viewport_rect.width () - output_rect.width ());
        break;
      case perserve_aspect_ratio_align::MID:
      default:
        output_rect.moveLeft (viewport_rect.x () + viewport_rect.width () * 0.5 - output_rect.width () * 0.5);
        break;
    }
  switch (m_y_align)
    {
      case perserve_aspect_ratio_align::MIN:
        output_rect.moveTop (viewport_rect.y ());
        break;
      case perserve_aspect_ratio_align::MAX:
        output_rect.moveTop (viewport_rect.y () + viewport_rect.height () - output_rect.height ());
        break;
      case perserve_aspect_ratio_align::MID:
      default:
        output_rect.moveTop (viewport_rect.y () + viewport_rect.height () * 0.5 - output_rect.height () * 0.5);
        break;
    }
  double bottom_cut = viewport_rect.x () - output_rect.x ();
  double top_cut = output_rect.x () + output_rect.width () - viewport_rect.x () - viewport_rect.width ();
  bottom_cut = bottom_cut > 0.0 ? bottom_cut : 0.0;
  top_cut = top_cut > 0.0 ? top_cut : 0.0;
  if (bottom_cut > 0.0 || 
      top_cut > 0.0)
    {
      bottom_cut /= x_ratio;
      top_cut /= x_ratio;
      original_rect.moveLeft (original_rect.x () + bottom_cut);
      original_rect.setWidth (original_rect.width () - bottom_cut - top_cut);
      output_rect.setX (viewport_rect.x ());
      output_rect.setWidth (viewport_rect.width ());
    }

  bottom_cut = viewport_rect.y () - output_rect.y ();
  top_cut = output_rect.y () + output_rect.height () - viewport_rect.y () - viewport_rect.height ();
  bottom_cut = bottom_cut > 0.0 ? bottom_cut : 0.0;
  top_cut = top_cut > 0.0 ? top_cut : 0.0;
  if (bottom_cut > 0.0 || 
      top_cut > 0.0)
    {
      bottom_cut /= y_ratio;
      top_cut /= y_ratio;
      original_rect.moveTop (original_rect.y () + bottom_cut);
      original_rect.setHeight (original_rect.height () - bottom_cut - top_cut);
      output_rect.setY (viewport_rect.y ());
      output_rect.setHeight (viewport_rect.height ());
    }
  return output_rect;
}

bool svg_attribute_preserve_aspect_ratio::write (QString &data, bool /*from_css*/) const
{
  data = QString ();
  if (m_defer)
    data += "defer ";
  data += "X";
  data += enum_to_string (m_x_align);
  data += "Y";
  data += enum_to_string (m_y_align);
  if (m_behaviour == perserve_aspect_ratio_behaviour::SLICE)
    data += " slice";
  return true;
}
