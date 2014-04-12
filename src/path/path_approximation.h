#ifndef PATH_APPROXIMATION_H
#define PATH_APPROXIMATION_H

class path_builder;
class svg_path;
class QPointF;

class path_approximation
{
  unique_ptr<path_builder> m_builder;
public:
  path_approximation (svg_path &path);
  ~path_approximation ();

  void approximate (std::vector<QPointF> points, double possible_error = 4.0);
private:
  void FitCubic (QPointF *d, int first, int last, QPointF tHat1, QPointF tHat2, double error);
  void output_curve (const QPointF *data);
};

#endif // PATH_APPROXIMATION_H
