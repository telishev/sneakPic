#ifndef RENDERER_PAGE_H
#define RENDERER_PAGE_H

#include "renderer/abstract_renderer_item.h"

class renderer_page : public abstract_renderer_item
{
  double m_height;
  double m_width;
public:
  renderer_page (const std::string &name);
  ~renderer_page ();

  virtual void draw (SkCanvas &canvas, const renderer_state &state, const renderer_config *config) const override;
  virtual QRectF bounding_box () const override;
  virtual void update_bbox () override;

  void set_height (double height) { m_height = height;}
  void set_width (double width) { m_width = width; }
};

#endif // RENDERER_PAGE_H
