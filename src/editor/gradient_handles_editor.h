#ifndef GRADIENT_HANDLES_EDITOR_H
#define GRADIENT_HANDLES_EDITOR_H

#include "editor/handles_editor.h"

class mouse_event_t;

class gradient_handles_editor : public handles_editor
{
  std::pair<std::string, int> m_selected_handle;
public:
  gradient_handles_editor (overlay_renderer *overlay, svg_painter *painter, actions_applier *applier);
  virtual ~gradient_handles_editor ();

  std::pair<std::string, int> selected_handle () const { return m_selected_handle; }

protected:
  virtual element_handles *create_handles_for_item (abstract_svg_item *item) override;
  virtual void update_handles_impl () override;

private:
  bool select_handle (const mouse_event_t &mevent);
};

#endif // GRADIENT_HANDLES_EDITOR_H
