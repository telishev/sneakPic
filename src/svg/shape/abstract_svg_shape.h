#ifndef ABSTRACT_SVG_SHAPE_H
#define ABSTRACT_SVG_SHAPE_H

#include <svg/point_2d.h>

class QPainterPath;

class abstract_svg_shape
{
  point_2d m_start;
  point_2d m_stop;

public:
  abstract_svg_shape (const point_2d &start, const point_2d &stop)
  {
    m_start = start;
    m_stop = stop;
  }
  virtual ~abstract_svg_shape () {}

  point_2d start () const { return m_start; }
  point_2d stop () const { return m_stop; }

  virtual void add_to_path (QPainterPath &shape) = 0;
};

#endif // ABSTRACT_SVG_SHAPE_H
