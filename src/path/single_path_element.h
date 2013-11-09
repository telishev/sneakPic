#ifndef SINGLE_PATH_ELEMENT_H
#define SINGLE_PATH_ELEMENT_H

#include <QPointF>

#include "path/geom_helpers.h"

class QTransform;

struct single_path_element
{
  QPointF start, end, c1, c2;
  single_path_element (const QPointF &start_point_, const QPointF &end_point_, const QPointF &first_control_, const QPointF &second_control_)
    : start (start_point_), end (end_point_), c1 (first_control_), c2 (second_control_) {}

  static single_path_element make_line (const QPointF &start, const QPointF &end)
  {
    QPointF first_control = (2 * start + end) / 3;
    QPointF second_control = (start + 2 * end) / 3;
    return single_path_element (start, end, first_control, second_control);
  }

  bool is_line () const
  {
    return    geom_helpers::are_line (start, end, c1)
           && geom_helpers::are_line (start, end, c2);
  }

  void apply_transform (const QTransform &transform);
};

#endif // SINGLE_PATH_ELEMENT_H