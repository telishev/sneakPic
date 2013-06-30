#ifndef RENDERER_ITEM_SVG_H
#define RENDERER_ITEM_SVG_H

#include "renderer/abstract_renderer_item.h"

class renderer_item_svg : public abstract_renderer_item
{
  double m_height;
  double m_width;
public:
  renderer_item_svg ();
  ~renderer_item_svg ();

  virtual void draw (QPainter &painter) const override;

  void set_height (double height);
  void set_width (double width);
};



#endif // RENDERER_ITEM_SVG_H
