#ifndef SINGLE_SUBPATH_H
#define SINGLE_SUBPATH_H

#include "path/single_path_element.h"

#include <vector>

class QTransform;


class single_subpath : public std::vector<single_path_element>
{
  bool m_is_closed;
  typedef std::vector<single_path_element> vec_t;

public:
  single_subpath ();
  ~single_subpath ();

  bool is_closed () const { return m_is_closed; }
  void set_closed (bool val) { m_is_closed = val; }

  bool first_point_is_last () const;

  QPointF first_point () const;
  QPointF last_point () const;

  QPointF point (size_t index) const;
  void set_point (size_t index, QPointF point);

  void apply_transform (const QTransform &transform);

};

#endif // SINGLE_SUBPATH_H
