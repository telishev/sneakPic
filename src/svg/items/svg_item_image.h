#ifndef SVG_ITEM_IMAGE_H
#define SVG_ITEM_IMAGE_H

#include "svg/items/svg_base_graphics_item.h"

class QImage;
class QPainterPath;

class svg_item_image : public svg_base_graphics_item
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
  virtual QPainterPath get_boundaries () const override;
  virtual renderer_graphics_item *create_renderer_graphics_item () const override;
};

#endif // SVG_ITEM_IMAGE_H
