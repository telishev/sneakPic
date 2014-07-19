#include "pencil_tool.h"

#include "gui/actions_applier.h"

#include "svg/svg_document.h"
#include "svg/items/svg_item_path.h"

#include "renderer/svg_painter.h"

#include "editor/operations/add_item_operation.h"
#include "editor/operations/path_edit_operation.h"

#include "path/path_approximation.h"
#include "path/svg_path.h"
#include "path/svg_path_geom.h"
#include "renderer/path_preview_renderer.h"

#include "skia/skia_includes.h"
#include "renderer/qt2skia.h"
#include "renderer/overlay_renderer.h"


class pencil_preview_renderer : public simple_path_renderer
{
  std::vector<QPointF> *m_current_points;
public:
  pencil_preview_renderer (std::vector<QPointF> *current_points, QColor color) : simple_path_renderer ({}, color)
  {
    m_current_points = current_points;
  }
  ~pencil_preview_renderer () {}

  virtual SkPath get_path () const override
  {
    if (m_current_points->empty ())
      return {};

    SkPath path;
    bool first = true;
    for (QPointF point : *m_current_points)
      {
        if (first)
          {
            first = false;
            path.moveTo (qt2skia::point (point));
          }
        else
          path.lineTo (qt2skia::point (point));
      }

    return path;
  }
};



pencil_tool::pencil_tool (svg_painter *painter)
  : pen_penctil_tool_base (painter)
{
  m_actions_applier->add_drag_shortcut (mouse_drag_shortcut_t::PENCIL, this,
    &pencil_tool::draw_pencil_start, &pencil_tool::draw_pencil_move, &pencil_tool::draw_pencil_end);

  put_in (m_preview_renderer, &m_current_points, QColor ("slateblue"));
  m_overlay->add_item (m_preview_renderer.get (), overlay_layer_type::TEMP);
}

pencil_tool::~pencil_tool ()
{

}

bool pencil_tool::draw_pencil_start (const QPoint &pos)
{
  if (!can_add_items ())
    return false;

  add_point (snap_point (pos));
  update ();
  return true;
}

bool pencil_tool::draw_pencil_move (const QPoint &pos)
{
  add_point (m_painter->get_local_pos (pos));
  update ();
  return true;
}

bool pencil_tool::draw_pencil_end (const QPoint &pos)
{
  add_point (snap_point (pos));
  finish_path_add ();
  return true;
}

void pencil_tool::set_new_path (svg_path *path)
{
  for (auto && point : m_current_points)
    point = m_painter->cur_transform ().map (point);

  path_approximation ().approximate (path, m_current_points, 10);
  path->get_geom ()->apply_transform (m_painter->cur_transform ().inverted ());
}

QString pencil_tool::undo_description () const
{
  return "Draw Freehand";
}

bool pencil_tool::edit_started () const
{
  return !m_current_points.empty ();
}

void pencil_tool::finish_editing_impl ()
{
  m_current_points.clear ();
}

void pencil_tool::add_point (QPointF local_pos)
{
  if (m_current_points.empty ())
    {
      m_current_points.push_back (local_pos);
      return;
    }

  const double min_distance = 2.0;
  const double max_distance = 20.0;
  QPointF prev_point = m_current_points.back ();
  double distance = geom::distance (prev_point, local_pos);
  if (distance < min_distance)
    return;

  int points_count = (int) (ceil (distance / max_distance));
  double increase = 1.0 / points_count;

  for (int i = 1; i <= points_count; i++)
    m_current_points.push_back (prev_point * (points_count - i) * increase + local_pos * i * increase);
}
