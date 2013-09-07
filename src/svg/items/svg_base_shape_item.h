#ifndef SVG_BASE_SHAPE_ITEM_H
#define SVG_BASE_SHAPE_ITEM_H

#include "svg/items/svg_base_graphics_item.h"

class QPainterPath;
class QTransform;
class QRectF;

class renderer_base_shape_item;

class svg_base_shape_item : public svg_base_graphics_item
{
public:
  svg_base_shape_item (svg_document *document);
  virtual ~svg_base_shape_item () override;

  QPainterPath get_path_for_clipping () const;

protected:
  virtual renderer_graphics_item *create_renderer_graphics_item () const override;
  void set_item_style (renderer_base_shape_item *item) const;
  virtual QPainterPath get_path () const = 0;
  virtual QPainterPath get_boundaries () const override;
  bool get_stroke (QPainterPath &dst) const;
  abstract_renderer_item *create_outline_renderer () const override;
};

#endif // SVG_BASE_SHAPE_ITEM_H
