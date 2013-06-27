#include "svg_subpath.h"

svg_subpath::svg_subpath ()
{

}


svg_subpath::~svg_subpath ()
{

}

void svg_subpath::add_shape (abstract_svg_shape *shape)
{
  m_shapes.push_back (shape);
}
