#ifndef HANDLES_RENDERER_H
#define HANDLES_RENDERER_H

#include "renderer/overlay_items_container.h"

class handles_editor;

class handles_renderer : public overlay_items_container
{
  const handles_editor *m_editor;
public:
  handles_renderer (const handles_editor *editor, overlay_renderer *overlay, svg_items_container *container);
  ~handles_renderer ();

protected:
  virtual renderable_item *create_overlay_item (const std::string &object) const override;
};

#endif // HANDLES_RENDERER_H
