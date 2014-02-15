#ifndef SVG_PATH_H
#define SVG_PATH_H

#include <QTransform>

class svg_path_geom;
enum class node_type_t : char;
class svg_path_geom_iterator;

class svg_path
{
  svg_path_geom *m_geom;
  vector<bool> *m_is_line_segment;
  vector<node_type_t> *m_node_type;
  QTransform m_transform;
public:
  svg_path (svg_path_geom *geom, vector<bool> *is_line_segment, vector<node_type_t> *node_type, QTransform transform);
  ~svg_path ();

  svg_path_geom *get_geom () { return m_geom; }
  vector<bool> *get_is_line_segment () { return m_is_line_segment; }
  vector<node_type_t> *get_node_type () { return m_node_type; }
  QTransform transform () const { return m_transform; }

  void move_anchor (QPointF dst, svg_path_geom_iterator it);
  void move_control_point (QPointF dst, svg_path_geom_iterator it, bool is_left);

  void copy_from (svg_path &src);

  void reverse_subpath (int subpath_index);
};

class unique_svg_path
{
  unique_ptr<svg_path_geom> m_geom;
  vector<bool> m_is_line_segment;
  vector<node_type_t> m_node_type;
  unique_ptr<svg_path> m_path;
public:
  unique_svg_path ();
  ~unique_svg_path ();

  svg_path *path () { return m_path.get (); }
  const svg_path *path () const { return m_path.get (); }
};

#endif // SVG_PATH_H
