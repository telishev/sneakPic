#ifndef SVG_BASE_SHAPE_ITEM_H
#define SVG_BASE_SHAPE_ITEM_H

#include "svg/items/abstract_svg_item.h"

class renderer_item_path;
class QPainterPath;
class QTransform;

class svg_base_shape_item : public abstract_svg_item
{
public:
  svg_base_shape_item (svg_document *document);
  virtual ~svg_base_shape_item () override;

  QPainterPath get_path_for_clipping () const;
  virtual abstract_renderer_item *create_renderer_item () const override;

protected:
  void set_item_style (renderer_item_path *item) const;
  virtual QPainterPath get_path () const = 0;
  QTransform full_transform () const;
};

#endif // SVG_BASE_SHAPE_ITEM_H
