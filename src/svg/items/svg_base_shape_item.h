#ifndef SVG_BASE_SHAPE_ITEM_H
#define SVG_BASE_SHAPE_ITEM_H

#include "svg/items/abstract_svg_item.h"

class renderer_base_shape_item;
class QPainterPath;
class QTransform;
class QRectF;

class svg_base_shape_item : public abstract_svg_item
{
public:
  svg_base_shape_item (svg_document *document);
  virtual ~svg_base_shape_item () override;

  QPainterPath get_path_for_clipping () const;
  virtual abstract_renderer_item *create_renderer_item () const override;
  virtual abstract_renderer_item *create_overlay_item (overlay_item_type overlay_type) const override;


protected:
  void set_item_style (renderer_base_shape_item *item) const;
  virtual QPainterPath get_path () const = 0;
  QTransform full_transform () const;
  QRectF exact_bbox (bool use_full_transform) const;
  QRectF bbox_with_clip (bool use_full_transform) const;
  bool get_stroke (QPainterPath &dst) const;
};

#endif // SVG_BASE_SHAPE_ITEM_H
