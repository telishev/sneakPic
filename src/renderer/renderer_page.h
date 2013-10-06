#ifndef RENDERER_PAGE_H
#define RENDERER_PAGE_H

#include "renderer/renderable_item.h"

class renderer_page : public renderable_item
{
  double m_height;
  double m_width;
public:
  renderer_page (double width, double height);
  ~renderer_page ();

  virtual void draw (SkCanvas &canvas, const renderer_state &state, const renderer_config *config) const override;
};

#endif // RENDERER_PAGE_H
