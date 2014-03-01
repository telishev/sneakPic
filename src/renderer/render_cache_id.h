#ifndef RENDER_CACHE_ID_H
#define RENDER_CACHE_ID_H

class QTransform;
class QRectF;

class render_cache_id_range;

#include <utility>

enum class render_cache_type
{
  INVALID = -1,
  ROOT_ITEM = 0,
  ROOT_ITEM_SELECTION,

  OBJECTS_ID_COUNT,
};

class render_cache_id
{
  int m_x, m_y, m_object_type;

public:
  render_cache_id () { m_x = m_y = 0; m_object_type = (int)render_cache_type::INVALID; }
  render_cache_id (int x_num, int y_num, int object_type) { m_x = x_num; m_y = y_num; m_object_type = object_type; }
  ~render_cache_id () {}

  static render_cache_id get_id_by_pixel_pos (double x_pos, double y_pos, const QTransform &transform, int object_type);
  static pair<render_cache_id, render_cache_id> get_id_for_pixel_rect (const QTransform &transform, const QRectF &rect, int object_type);
  QRectF pixel_rect (const QTransform &transform) const;

  int x () const { return m_x; }
  int y () const { return m_y; }
  int object_type () const { return m_object_type; }

  void set_id (int object_type) { m_object_type = object_type; }

  bool operator < (const render_cache_id &rhs) const
  {
    if (m_object_type != rhs.m_object_type)
      return m_object_type < rhs.m_object_type;

    if (m_x != rhs.m_x)
      return m_x < rhs.m_x;

    return m_y < rhs.m_y;
  }

  bool operator == (const render_cache_id &rhs) const
  {
    return m_x == rhs.m_x && m_y == rhs.m_y && m_object_type == rhs.m_object_type;
  }

  bool operator <= (const render_cache_id &rhs) const
  {
     return *this < rhs || *this == rhs;
  }
};


#endif // RENDER_CACHE_ID_H
