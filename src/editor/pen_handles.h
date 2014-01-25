#ifndef PEN_HANDLES_H
#define PEN_HANDLES_H

#include "editor/handles_editor.h"

class svg_path_iterator;
class svg_item_path;
class svg_path;

class pen_handles : public handles_editor
{
  svg_path *m_new_path;
public:
  pen_handles (overlay_renderer *overlay, svg_painter *painter, actions_applier *applier);
  ~pen_handles ();

  bool get_path_by_pos (QPointF screen_pos, svg_path_iterator &it, svg_item_path *&path);

  void set_new_path (svg_path *new_path);

  virtual void update_handles_impl () override;

protected:
  virtual element_handles *create_handles_for_item (abstract_svg_item *item) override;
};

#endif // PEN_HANDLES_H
