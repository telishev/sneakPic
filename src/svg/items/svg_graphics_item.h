#ifndef SVG_GRAPHICS_ITEM_H
#define SVG_GRAPHICS_ITEM_H

#include "svg/items/abstract_svg_item.h"

class renderer_graphics_item;
class QPainterPath;
class QTransform;
class QRectF;

class svg_graphics_item : public abstract_svg_item
{
public:
  svg_graphics_item (svg_document *document);
  virtual ~svg_graphics_item () override;

protected:
  QRectF exact_bbox (bool use_full_transform) const;
  QRectF bbox_with_clip (bool use_full_transform) const;
  QPainterPath get_path_for_clipping () const;
  QTransform full_transform () const;

  virtual QPainterPath get_boundaries () const = 0; // In this function path around item should be returned (even if it's raster image for example)
  virtual abstract_renderer_item *create_renderer_item () const override;
  virtual renderer_graphics_item *create_renderer_graphics_item () const = 0;
  virtual abstract_renderer_item *create_overlay_item (overlay_item_type overlay_type) const override;

private:
  void set_item_style (renderer_graphics_item *item) const;
};

#endif // SVG_GRAPHICS_ITEM_H
