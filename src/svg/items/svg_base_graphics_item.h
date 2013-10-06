#ifndef SVG_BASE_GRAPHICS_ITEM_H
#define SVG_BASE_GRAPHICS_ITEM_H

#include "svg/items/svg_graphics_item.h"

class renderer_graphics_item;
class QPainterPath;
class QTransform;
class QRectF;

class svg_base_graphics_item : public svg_graphics_item
{
public:
  svg_base_graphics_item (svg_document *document);
  virtual ~svg_base_graphics_item () override;

  virtual abstract_renderer_item *create_renderer_item_impl () const override;
  virtual void update_bbox_impl () override;
  virtual bool can_be_selected () const override { return true; }

protected:
  virtual QPainterPath get_boundaries () const = 0; // In this function path around item should be returned (even if it's raster image for example)
  virtual renderer_graphics_item *create_renderer_graphics_item () const = 0;

private:
  void set_item_style (renderer_graphics_item *item) const;
};

#endif // SVG_BASE_GRAPHICS_ITEM_H
