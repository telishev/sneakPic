#ifndef SVG_GRAPHICS_ITEM_H
#define SVG_GRAPHICS_ITEM_H

#include "svg/items/abstract_svg_item.h"

#include <QRectF>

class QTransform;

class svg_graphics_item : public abstract_svg_item
{
protected:
  QRectF m_bbox;
  bool m_bbox_valid;
public:
  svg_graphics_item (svg_document *document);
  ~svg_graphics_item ();

  virtual const svg_graphics_item *to_graphics_item () const override { return this; }
  virtual svg_graphics_item *to_graphics_item () override { return this; }

  abstract_renderer_item *create_renderer_item () const;
  virtual abstract_renderer_item *create_overlay_item (overlay_item_type overlay_type) const = 0;
  virtual bool can_be_selected () const = 0;

  void update_bbox ();
  void invalidate_bbox ();

  QRectF bbox () const { return m_bbox; }
  QTransform full_transform () const;

protected:
  virtual void update_bbox_impl () = 0;
  virtual abstract_renderer_item *create_renderer_item_impl () const = 0;
};

#endif // SVG_GRAPHICS_ITEM_H
