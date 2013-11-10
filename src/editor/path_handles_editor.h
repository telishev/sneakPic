#ifndef PATH_HANDLES_EDITOR_H
#define PATH_HANDLES_EDITOR_H

#include "editor/handles_editor.h"

class path_anchors_selection;
class mouse_event_t;

class path_handles_editor : public handles_editor
{
  path_anchors_selection *m_handles_selection;
public:
  path_handles_editor (overlay_renderer *overlay, svg_painter *painter, mouse_shortcuts_handler *mouse_handler);
  virtual ~path_handles_editor ();

  void begin_changes ();
  void end_changes ();
  void update ();

  path_anchors_selection *handles_selection () const { return m_handles_selection; }

protected:
  virtual element_handles *create_handles_for_item (abstract_svg_item *item) override;
  virtual void update_handles_impl () override;

private:
  bool select_handle (const mouse_event_t &mevent);


};

#endif // PATH_HANDLES_EDITOR_H
