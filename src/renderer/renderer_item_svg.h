#ifndef RENDERER_ITEM_SVG_H
#define RENDERER_ITEM_SVG_H

#include "renderer/renderer_item_group.h"

class renderer_item_svg : public renderer_item_group
{
  double m_height;
  double m_width; 
public:
  renderer_item_svg (const std::string &name);
  ~renderer_item_svg ();

  virtual void draw (SkCanvas &canvas, const renderer_state &state) const override;

  void set_height (double height);
  void set_width (double width);
};



#endif // RENDERER_ITEM_SVG_H
