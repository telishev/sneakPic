#ifndef RENDERER_ITEM_SVG_H
#define RENDERER_ITEM_SVG_H

#include "renderer/renderer_item_group.h"

class renderer_item_svg : public renderer_item_group
{
public:
  renderer_item_svg (const string &name);
  ~renderer_item_svg ();

  virtual void draw (SkCanvas &canvas, const renderer_state &state, const renderer_config *config) const override;
};



#endif // RENDERER_ITEM_SVG_H
