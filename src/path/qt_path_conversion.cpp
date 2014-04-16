#include "qt_path_conversion.h"

#include <QPainterPath>
#include "svg_path_geom.h"
#include "svg_path.h"
#include "path_builder.h"


QPainterPath qt_path_conversion::create_qpath (svg_path_geom *geom)
{
  QPainterPath path;
  for (const auto &subpath : geom->subpath ())
    {
      auto elements = subpath.elements ();
      path.moveTo (elements.front ().point);
      for (size_t i = 0; i < elements.size (); i++)
        {
          size_t next_element = i + 1;
          if (next_element == elements.size ())
            {
              if (!subpath.is_closed ())
                continue;
              else
                next_element = 0;
            }

          path.cubicTo (elements[i].c2, elements[next_element].c1, elements[next_element].point);
        }

      if (subpath.is_closed ())
        path.closeSubpath ();
    }
  return path;
}

void qt_path_conversion::create_from_qpath (QPainterPath qpath, svg_path &result)
{
  path_builder builder (result);
  int count = qpath.elementCount ();
  for (int i = 0; i < count; i++)
    {
      QPainterPath::Element elem = qpath.elementAt (i);
      switch  (elem.type)
        {
        case QPainterPath::MoveToElement:
          builder.move_to (elem, false);
          break;
        case QPainterPath::LineToElement:
          builder.line_to (elem, false);
          break;
        case QPainterPath::CurveToElement:
          {
            QPainterPath::Element control2 = qpath.elementAt (++i);
            QPainterPath::Element end = qpath.elementAt (++i);
            builder.curve_to (end, elem, control2, false);
            break;
          }
        default:
          break;
        }
    }

  if (count == 1)
    {
      builder.line_to (qpath.elementAt (0), false);
      builder.close_subpath ();
    }

  if (   count > 3
         && are_equal (qpath.elementAt (count - 1).x, qpath.elementAt (0).x)
         && are_equal (qpath.elementAt (count - 1).y, qpath.elementAt (0).y)
     )
    builder.close_subpath ();
  // Qt doesn't have any special definition if path is closed expect that it's first and last points are equal

}
