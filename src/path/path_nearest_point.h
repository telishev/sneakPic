#ifndef PATH_NEAREST_POINT_H
#define PATH_NEAREST_POINT_H

class QPointF;
class svg_path_geom;
class QTransform;
class svg_path_geom_iterator;

class path_nearest_point
{
public:
  double get_nearest_point (QPointF screen_pos, QTransform transform, const svg_path_geom *path,
                            double *t = nullptr, svg_path_geom_iterator *res_it = nullptr);
  QPointF get_nearest_point (QPointF pos, QPointF curve[4], double *t = nullptr);
};

#endif // PATH_NEAREST_POINT_H
