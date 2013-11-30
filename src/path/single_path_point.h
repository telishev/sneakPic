#ifndef SINGLE_PATH_ELEMENT_H
#define SINGLE_PATH_ELEMENT_H

#include <QPointF>
#include "geom_helpers.h"
class QTransform;

struct single_path_point
{
  QPointF point, c1, c2;
  single_path_point (const QPointF &point_, const QPointF &c1_, const QPointF &c2_ )
    : point (point_), c1 (c1_), c2 (c2_) {}

  void apply_transform (const QTransform &transform);
};

struct single_path_segment
{
  QPointF start, end, c1, c2;

  single_path_segment () {}
  single_path_segment (const QPointF &start_, const QPointF &end_, const QPointF &c1_, const QPointF &c2_ )
    : start (start_), end (end_), c1 (c1_), c2 (c2_) {}


  bool is_line () const { return geom::are_line (start, c1, end) && geom::are_line (start, c2, end); }
};


#endif // SINGLE_PATH_ELEMENT_H