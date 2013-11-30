#include "single_path_point.h"

#include <QTransform>


void single_path_point::apply_transform (const QTransform &transform)
{
  point = transform.map (point);
  c1 = transform.map (c1);
  c2 = transform.map (c2);
}
