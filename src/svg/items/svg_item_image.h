#ifndef SVG_ITEM_IMAGE_H
#define SVG_ITEM_IMAGE_H

#include "svg/items/abstract_svg_item.h"

class QImage;

class svg_item_image : public abstract_svg_item
{
  SVG_ITEM

public:
  svg_item_image (svg_document *document);
  virtual ~svg_item_image () override;

protected:
  double m_x, m_y;
  double m_width, m_height;

protected:
  virtual bool check_item () override;
  abstract_renderer_item *create_renderer_item () const override;
  abstract_renderer_item *svg_item_image::create_overlay_item () const override;
};

#endif // SVG_ITEM_IMAGE_H
