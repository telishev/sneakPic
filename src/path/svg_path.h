#ifndef SVG_PATH_H
#define SVG_PATH_H

#include "path/single_subpath.h"

class QTransform;
class svg_path_iterator;

class svg_path 
{
  std::vector<single_subpath> m_subpath;
public:
  svg_path ();
  ~svg_path ();

  size_t total_points () const;
  size_t total_segments () const;

  /// i'm too lazy to write const iterator(
  /// TODO: write const iterator or something
  svg_path_iterator begin () const;
  svg_path_iterator end () const;
  svg_path_iterator last_point () const;

  void clear ();

  void apply_transform (const QTransform &transform);

  const std::vector<single_subpath> &subpath () const { return m_subpath; }
  std::vector<single_subpath> &subpath () { return m_subpath; }

  friend class svg_path_iterator;
  friend class path_builder;
};

class svg_path_iterator
{
  svg_path *m_path;
  size_t m_subpath_index;
  subpath_iterator m_subpath_iterator;
public:
  svg_path_iterator ();
  svg_path_iterator (svg_path &path, size_t subpath_index, subpath_iterator iterator);
  svg_path_iterator (svg_path &path, size_t subpath_index, size_t subpath_point);
  ~svg_path_iterator ();

  svg_path_iterator &operator ++ ();
  bool operator == (const svg_path_iterator &rhs) const;
  bool operator != (const svg_path_iterator &rhs) const;

  bool is_valid () const { return m_path != 0; }

  svg_path_iterator neighbour (bool is_left) const;
  svg_path_iterator left () const;
  svg_path_iterator right () const;

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


#endif // SVG_PATH_H
