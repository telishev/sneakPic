#ifndef SVG_ARC_DATA_H
#define SVG_ARC_DATA_H


#include <qglobal.h>

class path_builder;

class arc_converter
{
public:
  static void pathArc (path_builder &builder,
                       qreal               rx,
                       qreal               ry,
                       qreal               x_axis_rotation,
                       int         large_arc_flag,
                       int         sweep_flag,
                       qreal               x,
                       qreal               y,
                       qreal curx, qreal cury);
};

#endif // SVG_ARC_DATA_H