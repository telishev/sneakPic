#include "path/single_subpath.h"

single_subpath::single_subpath ()
{
  m_is_closed = false;
}

single_subpath::~single_subpath ()
{

}

QPointF single_subpath::first_point () const
{
  return front ().start_point;
}
