#include "svg/attributes/svg_attribute_stroke_linecap.h"

svg_attribute_stroke_linecap::svg_attribute_stroke_linecap (svg_document *document)
  : abstract_attribute (document)
{
  m_linecap = Qt::PenCapStyle::FlatCap;
}

svg_attribute_stroke_linecap::~svg_attribute_stroke_linecap ()
{
}

bool svg_attribute_stroke_linecap::read (const char *data, bool /*from_css*/)
{
  m_linecap = Qt::PenCapStyle::FlatCap;
  if (QLatin1String (data) == "round")
    m_linecap = Qt::PenCapStyle::RoundCap;
  else if (QLatin1String (data) == "square")
    m_linecap = Qt::PenCapStyle::SquareCap;
  return true;
}

bool svg_attribute_stroke_linecap::write (QString &data, bool /*from_css*/) const
{
  switch (m_linecap)
    {
      case Qt::PenCapStyle::FlatCap:
        data = "butt";
        break;
      case Qt::PenCapStyle::RoundCap:
        data = "round";
        break;
      case Qt::PenCapStyle::SquareCap:
        data = "square";
        break;
      default:
        return false;
    }
  return true;
}