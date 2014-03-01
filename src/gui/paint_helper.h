#ifndef PAINT_HELPER_H
#define PAINT_HELPER_H

class QPainter;

class paint_helper
{
public:
  void draw_checkerboard (QPainter &painter, double width, double height);
  void draw_border (QPainter &painter, double width, double height);

public:
  static const int BORDER_WIDTH = 1;
};

#endif // PAINT_HELPER_H
