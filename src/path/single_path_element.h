#ifndef SINGLE_PATH_ELEMENT_H
#define SINGLE_PATH_ELEMENT_H

#include <QPointF>

#include "path/geom_helpers.h"

struct single_path_element
{
  QPointF start_point, end_point, first_control, second_control;
  single_path_element (const QPointF &start_point_, const QPointF &end_point_, const QPointF &first_control_, const QPointF &second_control_)
    : start_point (start_point_), end_point (end_point_), first_control (first_control_), second_control (second_control_) {}

  static single_path_element make_line (const QPointF &start, const QPointF &end)
  {
    QPointF first_control = (2 * start + end) / 3;
    QPointF second_control = (start + 2 * end) / 3;
    return single_path_element (start, end, first_control, second_control);
  }

  bool is_line () const
  {
    return    geom_helpers::are_line (start_point, end_point, first_control)
           && geom_helpers::are_line (start_point, end_point, second_control);
  }
};

#endif // SINGLE_PATH_ELEMENT_H