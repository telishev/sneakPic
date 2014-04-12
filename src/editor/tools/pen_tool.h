#ifndef PEN_TOOL_H
#define PEN_TOOL_H

#include "editor/tools/pen_penctil_tool_base.h"

class path_preview_renderer;
class unique_svg_path;
class path_builder;
class anchor_handle_renderer;
class path_control_point_renderer;
class pen_handles;
class svg_path_geom_iterator;
class svg_item_path;

class pen_tool : public pen_penctil_tool_base
{
  Q_OBJECT

  unique_ptr<path_preview_renderer> m_preview_renderer;
  unique_ptr<path_preview_renderer> m_auxiliary_preview_renderer;
  unique_ptr<path_control_point_renderer> m_left_cp_renderer;
  unique_ptr<path_control_point_renderer> m_right_cp_renderer;
  unique_ptr<unique_svg_path> m_current_path;
  unique_ptr<unique_svg_path> m_auxiliary_path;
  unique_ptr<path_builder> m_path_builder;

  bool m_prev_point_was_line;

public:
  pen_tool (svg_painter *painter);
  ~pen_tool ();

protected:
  virtual void set_new_path (svg_path *path) override;
  virtual QString undo_description () const override;
  virtual bool edit_started () const override;
  virtual void finish_editing_impl () override;

private:
  bool add_segment_simple (const QPoint &pos);
  bool add_segment_start (const QPoint &pos);
  bool add_segment_move (const QPoint &pos);
  bool add_segment_end (const QPoint &pos);
  bool update_auxiliary_pen_preview (const QPoint &pos = QPoint ());
  bool cancel_curve ();
  void update_cp_renderers ();

  void add_new_point (QPoint pos, bool is_line);
private slots:
  void update_on_settings_changed ();
};

#endif // PEN_TOOL_H

