#ifndef SVG_PATH_H
#define SVG_PATH_H

#include "path/single_subpath.h"

class QTransform;

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
  void move_point (size_t index, QPointF point);

  void get_subpath_and_index (size_t global_index, const single_subpath *&subpath, size_t &index) const;
  void get_subpath_and_index (size_t global_index, single_subpath *&subpath, size_t &index);

  void apply_transform (const QTransform &transform);

  bool control_point_exists (size_t index, bool left_cp) const;
  QPointF control_point (size_t index, bool left_cp) const;
  void set_control_point (size_t index, bool left_cp, QPointF point);

  size_t prev_point (size_t index) const;
  size_t next_point (size_t index) const;

private:
  size_t global_index (size_t local_index, const single_subpath *subpath) const;
};

#endif // SVG_PATH_H
