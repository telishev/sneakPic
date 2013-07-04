#ifndef SVG_ITEM_CIRCLE_H
#define SVG_ITEM_CIRCLE_H

#include "svg/items/svg_base_shape_item.h"

class renderer_item_ellipse;

class svg_item_circle : public svg_base_shape_item
{
  SVG_ITEM

  renderer_item_ellipse *m_render_item;
public:
  svg_item_circle (svg_document *document);
  virtual ~svg_item_circle () override;

  virtual void update_renderer_item () override;
  virtual const abstract_renderer_item *get_renderer_item () const override;

protected:
  virtual bool check_item () override;
  virtual QPainterPath get_path () const override;
};


#endif // SVG_ITEM_CIRCLE_H
