#ifndef QT_PATH_CONVERSION_H
#define QT_PATH_CONVERSION_H

class QPainterPath;
class svg_path_geom;
class svg_path;

class qt_path_conversion
{
public:
  QPainterPath create_qpath (svg_path_geom *geom);
  void create_from_qpath (QPainterPath qpath, svg_path &result);
};

#endif // QT_PATH_CONVERSION_H
