#ifndef RENDER_CACHE_ID_H
#define RENDER_CACHE_ID_H

class QTransform;
class QRectF;

class render_cache_id_range;

class render_cache_id
{
  int m_x, m_y, m_id;

public:
  enum object_id
  {
    INVALID = -1,
    CURRENT_SCREEN = 0,
    ROOT_ITEM,
    ROOT_ITEM_SELECTION,

    OBJECTS_ID_COUNT,
  };

public:
  render_cache_id () { m_x = m_y = 0; m_id = INVALID; }
  render_cache_id (int x_num, int y_num, int id) { m_x = x_num; m_y = y_num; m_id = id; }
  ~render_cache_id () {}

  static render_cache_id current_screen_id () { return render_cache_id (0, 0, CURRENT_SCREEN); }

  static render_cache_id get_id_by_pos (double x_pos, double y_pos, const QTransform &transform);
  QRectF local_rect (const QTransform &transform) const;
  QRectF pixel_rect (const QTransform &transform) const;

  int x () const { return m_x; }
  int y () const { return m_y; }
  int id () const { return m_id; }

  void set_id (int id) { m_id = id; }

  bool operator < (const render_cache_id &rhs) const
  {
    if (m_id != rhs.m_id)
      return m_id < rhs.m_id;

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
