#ifndef PEN_TOOL_H
#define PEN_TOOL_H

#include "editor/tools/abstract_tool.h"

class path_preview_renderer;
class svg_path;
class path_builder;
class anchor_handle_renderer;
class path_control_point_renderer;

class pen_tool : public abstract_tool
{
  std::unique_ptr<path_preview_renderer> m_preview_renderer;
  std::unique_ptr<anchor_handle_renderer> m_anchor_renderer;
  std::unique_ptr<path_control_point_renderer> m_left_cp_renderer;
  std::unique_ptr<path_control_point_renderer> m_right_cp_renderer;
  std::unique_ptr<svg_path> m_current_path;
  std::unique_ptr<path_builder> m_path_builder;
public:
  pen_tool (svg_painter *painter);
  ~pen_tool ();

  virtual void deactivate () override;

private:
  bool add_segment_simple (const QPoint &pos);
  bool add_segment_start (const QPoint &pos);
  bool add_segment_move (const QPoint &pos);
  bool add_segment_end (const QPoint &pos);
  bool finish_path_add ();
};


#endif // PEN_TOOL_H
