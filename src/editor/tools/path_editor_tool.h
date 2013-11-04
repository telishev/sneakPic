#ifndef PATH_EDITOR_TOOL_H
#define PATH_EDITOR_TOOL_H

#include "editor/tools/abstract_tool.h"

class mouse_shortcuts_handler;
class svg_painter;
class rubberband_selection;
class overlay_renderer;

class path_editor_tool : public abstract_tool
{
  rubberband_selection    *m_rubberband;

public:
  path_editor_tool (svg_painter *painter);
  ~path_editor_tool ();

private:
  bool delete_handles ();
};

#endif // PATH_EDITOR_TOOL_H
