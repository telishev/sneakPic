#ifndef POINT_2D_H
#define POINT_2D_H

class point_2d
{
  double m_x;
  double m_y;
public:
  point_2d () { m_x = 0; m_y = 0; }
  point_2d (double x, double y) { m_x = x; m_y = y; }
  ~point_2d () {}

  point_2d operator + (const point_2d &rhs) { return point_2d (m_x + rhs.m_x, m_y + rhs.m_y); }
  point_2d operator - (const point_2d &rhs) { return point_2d (m_x - rhs.m_x, m_y - rhs.m_y); }

  point_2d &operator += (const point_2d &rhs) { m_x += rhs.m_x; m_y += rhs.m_y; return *this; }
  point_2d &operator -= (const point_2d &rhs) { m_x -= rhs.m_x; m_y -= rhs.m_y; return *this; }

  double x () const { return m_x; }
  double y () const { return m_y; }
};

#endif // POINT_2D_H
