#ifndef PATH_APPROXIMATION_H
#define PATH_APPROXIMATION_H

class path_builder;
class svg_path;
class QPointF;

class path_approximation
{
  unique_ptr<path_builder> m_builder;
public:
  path_approximation ();
  ~path_approximation ();

  void approximate (svg_path *path, std::vector<QPointF> points, double possible_error = 4.0);
  void fit_segment (std::vector<QPointF> points, QPointF output[4]);
  QPointF bezier_value (QPointF bezier[4], double t) const;

private:
  void FitCubic (QPointF *d, int first, int last, QPointF tHat1, QPointF tHat2, double error);
  void output_curve (const QPointF *data);
};

#endif // PATH_APPROXIMATION_H
