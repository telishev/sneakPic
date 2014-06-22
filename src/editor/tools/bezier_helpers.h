#ifndef BEZIER_HELPERS_H
#define BEZIER_HELPERS_H

struct single_path_segment;
class QPointF;

class bezier_helpers
{
public:
  QPointF get_pos_by_value (const single_path_segment &segment, double t) const;
  QPointF get_tangent_by_value (const single_path_segment &segment, double t) const;
  std::vector<single_path_segment> split (const single_path_segment &segment, double t) const;
};

#endif // BEZIER_HELPERS_H
