#ifndef SVG_PATH_GEOM_H
#define SVG_PATH_GEOM_H

#include "path/single_subpath.h"

class QTransform;
class svg_path_geom_iterator;

class svg_path_geom
{
  std::vector<single_subpath> m_subpath;
public:
  svg_path_geom ();
  ~svg_path_geom ();

  size_t total_points () const;
  size_t total_segments () const;

  /// i'm too lazy to write const iterator(
  /// TODO: write const iterator or something
  svg_path_geom_iterator begin () const;
  svg_path_geom_iterator end () const;
  svg_path_geom_iterator last_point () const;

  svg_path_geom_iterator subpath_begin (int subpath_index) const;
  svg_path_geom_iterator subpath_last_point (int subpath_index) const;

  void clear ();

  void apply_transform (const QTransform &transform);

  const std::vector<single_subpath> &subpath () const { return m_subpath; }
  std::vector<single_subpath> &subpath () { return m_subpath; }
  void erase (svg_path_geom_iterator iterator);

  friend class svg_path_geom_iterator;
  friend class path_builder;
};

class svg_path_geom_iterator
{
  svg_path_geom *m_path;
  size_t m_subpath_index;
  subpath_iterator m_subpath_iterator;
public:
  svg_path_geom_iterator ();
  svg_path_geom_iterator (svg_path_geom &path, size_t subpath_index, subpath_iterator iterator);
  svg_path_geom_iterator (svg_path_geom &path, size_t subpath_index, size_t subpath_point);
  ~svg_path_geom_iterator ();

  svg_path_geom_iterator &operator ++ ();
  bool operator == (const svg_path_geom_iterator &rhs) const;
  bool operator != (const svg_path_geom_iterator &rhs) const;

  bool is_valid () const { return m_path != 0; }

  svg_path_geom_iterator neighbour (bool is_left) const;
  svg_path_geom_iterator left () const;
  svg_path_geom_iterator right () const;

  const QPointF &anchor_point () const;
  QPointF &anchor_point ();

  const QPointF &control_point (bool is_left) const;
  QPointF &control_point (bool is_left);
  bool has_control_point (bool is_left);

  size_t point_index () const;
  int segment_index (bool is_left) const;
  single_path_segment segment (bool is_left) const;

  single_subpath &subpath ();
  size_t get_subpath_index () const;
  size_t get_subpath_point () const;
};


#endif // SVG_PATH_GEOM_H
