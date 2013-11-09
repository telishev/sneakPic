#include "single_path_element.h"

#include <QTransform>


void single_path_element::apply_transform (const QTransform &transform)
{
  start = transform.map (start);
  end = transform.map (end);
  c1 = transform.map (c1);
  c2 = transform.map (c2);
}
