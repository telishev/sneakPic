#ifndef PEN_PENCTIL_TOOL_BASE_H
#define PEN_PENCTIL_TOOL_BASE_H

#include "editor/tools/abstract_tool.h"

class path_preview_renderer;
class pen_handles;
class svg_path_geom_iterator;
class svg_item_path;
class svg_path;

class pen_penctil_tool_base : public abstract_tool
{
protected:
  typedef pair<svg_item_path *, svg_path_geom_iterator> snap_point_t;
  unique_ptr<pen_handles> m_pen_handles;

  unique_ptr<snap_point_t> m_path_snap_start;
  unique_ptr<snap_point_t> m_path_snap_end;

public:
  pen_penctil_tool_base (svg_painter *painter);
  ~pen_penctil_tool_base ();

  virtual void configure () override;
  virtual void deactivate () override;
  virtual void activate () override;

protected:
  virtual void set_new_path (svg_path *path) = 0;
  virtual QString undo_description () const = 0;
  virtual bool edit_started () const = 0;
  virtual void finish_editing_impl () = 0;

  bool finish_path_add ();
  QPointF snap_point (QPointF point);
  bool cancel_editing ();
  void finish_editing ();
private:
  svg_item_path *add_new_path ();
  svg_item_path *merge_with_path (svg_item_path *path_dst, svg_path_geom_iterator dst_it, svg_item_path *path_src, svg_path_geom_iterator src_it);
};

#endif // PEN_PENCTIL_TOOL_BASE_H
