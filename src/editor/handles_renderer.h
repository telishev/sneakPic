#ifndef HANDLES_RENDERER_H
#define HANDLES_RENDERER_H

#include "renderer/renderable_item.h"

class handles_editor;

class handles_renderer : public renderable_item
{
  const handles_editor *m_editor;
public:
  handles_renderer (const handles_editor *editor);
  ~handles_renderer ();

protected:
  virtual void draw (SkCanvas &canvas, const renderer_state &state, const renderer_config *config) const override;
  
};

#endif // HANDLES_RENDERER_H
