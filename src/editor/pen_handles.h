#ifndef PEN_HANDLES_H
#define PEN_HANDLES_H

#include "editor/selection_handles_editor.h"

class svg_path_geom_iterator;
class svg_item_path;
class svg_path_geom;

class pen_handles : public selection_handles_editor
{
  svg_path_geom *m_new_path;
public:
  pen_handles (overlay_renderer *overlay, svg_painter *painter, actions_applier *applier);
  ~pen_handles ();

  bool get_path_by_pos (QPointF screen_pos, svg_path_geom_iterator &it, svg_item_path *&path);

  void set_new_path (svg_path_geom *new_path);

  virtual void update_handles_impl () override;

protected:
  virtual element_handles *create_handles_for_item (abstract_svg_item *item) override;
};

#endif // PEN_HANDLES_H
