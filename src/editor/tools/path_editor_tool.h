#ifndef PATH_EDITOR_TOOL_H
#define PATH_EDITOR_TOOL_H

#include "editor/tools/abstract_tool.h"

class mouse_shortcuts_handler;
class svg_painter;
class rubberband_selection;
class overlay_renderer;
class path_handles_editor;
class gradient_handles_editor;

class path_editor_tool : public abstract_tool
{
  unique_ptr<rubberband_selection>    m_rubberband;
  unique_ptr<path_handles_editor>     m_path_editor;
  unique_ptr<gradient_handles_editor> m_gradient_editor;

public:
  path_editor_tool (svg_painter *painter);
  ~path_editor_tool ();

private:
  virtual void configure () override;
  virtual void activate () override;
  virtual void deactivate () override;

  void update_handles ();

};

#endif // PATH_EDITOR_TOOL_H
