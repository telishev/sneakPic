#ifndef SVG_SUBPATH_H
#define SVG_SUBPATH_H

class abstract_svg_shape;

#include <vector>

class svg_subpath
{
  std::vector<abstract_svg_shape *> m_shapes;
public:
  svg_subpath ();
  ~svg_subpath ();

  abstract_svg_shape *front () const { return m_shapes.front (); }
  abstract_svg_shape *back () const { return m_shapes.back (); }
  bool empty () const { return m_shapes.empty (); }

  void add_shape (abstract_svg_shape *shape);

  const std::vector<abstract_svg_shape *> &shapes () const { return m_shapes; }
};

#endif // SVG_SUBPATH_H
