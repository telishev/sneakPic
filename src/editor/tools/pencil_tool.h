#ifndef PENCIL_TOOL_H
#define PENCIL_TOOL_H

#include "editor/tools/pen_penctil_tool_base.h"
class pencil_preview_renderer;

class pencil_tool : public pen_penctil_tool_base
{
  unique_ptr<pencil_preview_renderer> m_preview_renderer;
  std::vector<QPointF> m_current_points;

public:
  pencil_tool (svg_painter *painter);
  ~pencil_tool ();

private:
  bool draw_pencil_start (const QPoint &pos);
  bool draw_pencil_move (const QPoint &pos);
  bool draw_pencil_end (const QPoint &pos);

  virtual void set_new_path (svg_path *path) override;
  virtual QString undo_description () const override;
  virtual bool edit_started () const override;
  virtual void finish_editing_impl () override;

};

#endif // PENCIL_TOOL_H
