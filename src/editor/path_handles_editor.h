#ifndef PATH_HANDLES_EDITOR_H
#define PATH_HANDLES_EDITOR_H

#include "editor/handles_editor.h"

class path_handles_editor : public handles_editor
{
public:
  path_handles_editor (overlay_renderer *overlay, svg_painter *painter, mouse_shortcuts_handler *mouse_handler);
  virtual ~path_handles_editor ();


protected:
  virtual element_handles *create_handles_for_item (abstract_svg_item *item) override;


};

#endif // PATH_HANDLES_EDITOR_H
