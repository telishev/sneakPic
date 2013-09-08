#ifndef SVG_PAGE_RENDERER_H
#define SVG_PAGE_RENDERER_H

#include "renderer/overlay_items_container.h"

class svg_page_renderer : public overlay_items_container
{
  double m_width, m_height;
public:
  svg_page_renderer (overlay_renderer *overlay);
  ~svg_page_renderer ();

  void set_dimensions (double width, double height);

protected:
  virtual std::vector<abstract_renderer_item *> create_overlay_item (const std::string &object) const override;
};


#endif // SVG_PAGE_RENDERER_H