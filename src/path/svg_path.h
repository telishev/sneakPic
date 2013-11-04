#ifndef SVG_PATH_H
#define SVG_PATH_H

#include "path/single_subpath.h"

class svg_path : public std::vector<single_subpath>
{
  typedef std::vector<single_subpath> vec_t;
public:
  svg_path ();
  ~svg_path ();

  size_t total_elements () const;
  single_path_element *element (size_t index);

  size_t total_points () const;
  QPointF point (size_t index) const;
  void set_point (size_t index, QPointF point);

};

#endif // SVG_PATH_H
