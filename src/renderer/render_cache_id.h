#ifndef RENDER_CACHE_ID_H
#define RENDER_CACHE_ID_H

class QTransform;
class QRectF;

class render_cache_id_range;

class render_cache_id
{
  int m_x, m_y;

public:
  render_cache_id (int x_num = 0, int y_num = 0) { m_x = x_num; m_y = y_num; }
  ~render_cache_id () {}

  static render_cache_id get_id_by_pos (double x_pos, double y_pos, const QTransform &transform);
  QRectF local_rect (const QTransform &transform) const;
  QRectF pixel_rect (const QTransform &transform) const;

  int x () const { return m_x; }
  int y () const { return m_y; }

  bool operator < (const render_cache_id &rhs) const
  {
    if (m_x != rhs.m_x)
      return m_x < rhs.m_x;

    return m_y < rhs.m_y;
  }

  bool operator == (const render_cache_id &rhs) const
  {
    return m_x == rhs.m_x && m_y == rhs.m_y;
  }

  bool operator <= (const render_cache_id &rhs) const
  {
     return *this < rhs || *this == rhs;
  }
};


#endif // RENDER_CACHE_ID_H
