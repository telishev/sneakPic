#include "svg/attributes/svg_attribute_stroke_linejoin.h"

svg_attribute_stroke_linejoin::svg_attribute_stroke_linejoin (abstract_svg_item *item)
  : abstract_attribute (item)
{
  m_linejoin = Qt::PenJoinStyle::SvgMiterJoin;
}

svg_attribute_stroke_linejoin::~svg_attribute_stroke_linejoin ()
{
}

bool svg_attribute_stroke_linejoin::read (const QString &data, bool /*from_css*/)
{
  m_linejoin = Qt::PenJoinStyle::SvgMiterJoin;
  if (data == "round")
    m_linejoin = Qt::PenJoinStyle::RoundJoin;
  else if (data == "bevel")
    m_linejoin = Qt::PenJoinStyle::BevelJoin;
  return true;
}

bool svg_attribute_stroke_linejoin::write (QString &data, bool /*from_css*/) const
{
  switch (m_linejoin)
    {
  case Qt::PenJoinStyle::SvgMiterJoin:
        data = "miter";
        break;
      case Qt::PenJoinStyle::RoundJoin:
        data = "round";
        break;
      case Qt::PenJoinStyle::BevelJoin:
        data = "bevel";
        break;
      default:
        return false;
    }
  return true;
}