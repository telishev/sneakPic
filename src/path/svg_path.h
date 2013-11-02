#ifndef SVG_PATH_H
#define SVG_PATH_H

#include "path/single_subpath.h"

class svg_path : public std::vector<single_subpath>
{
  typedef std::vector<single_subpath> vec_t;
public:
  svg_path ();
  ~svg_path ();

  using vec_t::begin;
  using vec_t::end;
  using vec_t::operator[];
  using vec_t::resize;
  using vec_t::reserve;
  using vec_t::push_back;
  using vec_t::at;
  using vec_t::insert;
  using vec_t::erase;
  using vec_t::empty;
  using vec_t::size;
  using vec_t::back;
  using vec_t::front;

  using vec_t::iterator;
  using vec_t::const_iterator;
};

#endif // SVG_PATH_H
