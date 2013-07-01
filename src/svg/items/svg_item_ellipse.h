#ifndef SVG_ITEM_ELLIPSE_H
#define SVG_ITEM_ELLIPSE_H

#include "svg/items/svg_base_shape_item.h"

class renderer_item_ellipse;

class svg_item_ellipse : public svg_base_shape_item
{
  SVG_ITEM

  renderer_item_ellipse *m_render_item;
public:
  svg_item_ellipse (svg_document *document);
  virtual ~svg_item_ellipse () override;

  virtual void update_renderer_item ();
  virtual const abstract_renderer_item *get_renderer_item () const;

protected:
  virtual bool check_item () override;

};


#endif // SVG_ITEM_ELLIPSE_H
