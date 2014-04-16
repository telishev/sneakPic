#ifndef SVG_BASE_SHAPE_ITEM_H
#define SVG_BASE_SHAPE_ITEM_H

#include "svg/items/svg_base_graphics_item.h"

class QPainterPath;
class QTransform;
class QRectF;

class renderer_base_shape_item;
class svg_base_attribute_marker_usage;

class svg_base_shape_item : public svg_base_graphics_item
{
public:
  svg_base_shape_item (svg_document *document);
  virtual ~svg_base_shape_item () override;

  QPainterPath get_path_for_clipping () const;
  virtual QPainterPath get_path () const = 0;

protected:
  virtual QPainterPath get_boundaries () const override;
  virtual renderer_graphics_item *create_renderer_graphics_item () const override;
  renderable_item *create_outline_renderer () const override;

  
  void set_item_style (renderer_base_shape_item *item) const;
  bool get_stroke (QPainterPath &dst) const;
  void configure_markers_on_path_drawing (renderer_base_shape_item *base_item,
    const svg_base_attribute_marker_usage *marker, const QPainterPath &path, const QTransform &transform, double stroke_width) const;
  void configure_markers (renderer_base_shape_item *item) const;
};

#endif // SVG_BASE_SHAPE_ITEM_H
