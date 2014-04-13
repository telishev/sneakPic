#ifndef PATH_HANDLES_EDITOR_H
#define PATH_HANDLES_EDITOR_H

#include "editor/selection_handles_editor.h"

class path_anchors_selection;
class rubberband_selection;
class mouse_event_t;
class QRectF;
class path_anchor_handle;

class path_handles_editor : public selection_handles_editor
{
  path_anchors_selection *m_handles_selection;
  rubberband_selection *m_rubberband;
  bool m_disable_deselect;
public:
  path_handles_editor (overlay_renderer *overlay, svg_painter *painter, actions_applier *applier);
  virtual ~path_handles_editor ();

  void begin_changes ();
  void end_changes (QString name);
  void update ();

  path_anchors_selection *handles_selection () const { return m_handles_selection; }

protected:
  virtual element_handles *create_handles_for_item (abstract_svg_item *item) override;
  virtual void update_handles_impl () override;

private:
  bool select_handle (const mouse_event_t &mevent);
  bool change_handle (const QPoint &pos);
  void select_by_rect (const QRectF &rect);
  bool delete_selected_handles ();
  path_anchor_handle *get_path_anchor (const QPoint &pos) const;
  bool deselect_handles ();
  void deselect_other ();
};

#endif // PATH_HANDLES_EDITOR_H
