#ifndef RENDERER_ITEM_SVG_H
#define RENDERER_ITEM_SVG_H

#include "renderer/renderer_item_group.h"

class renderer_item_svg : public renderer_item_group
{
  double m_height;
  double m_width; 
public:
  renderer_item_svg (const abstract_svg_item *m_svg_item);
  ~renderer_item_svg ();

  virtual void draw (QPainter &painter, const renderer_state &state) const override;

  void set_height (double height);
  void set_width (double width);
};



#endif // RENDERER_ITEM_SVG_H
