#ifndef RENDERER_ITEM_GROUP_H
#define RENDERER_ITEM_GROUP_H

#include <renderer/abstract_renderer_item.h>
class abstract_svg_item;

class renderer_item_group : public abstract_renderer_item
{
  double m_opacity;
  mutable QRectF m_bbox;
  mutable bool m_bbox_calculated;
  const abstract_svg_item *m_svg_item;

public:
  renderer_item_group (const abstract_svg_item *svg_item);
  ~renderer_item_group ();

  virtual void draw (QPainter &painter) const override;
  virtual QRectF bounding_box () const override;

  virtual double group_opacity () const override { return m_opacity; }
  void set_opacity (double opacity) { m_opacity = opacity; }

  void invalidate_bbox () { m_bbox_calculated = false; }
private:
  void update_bbox () const;
};

#endif // RENDERER_ITEM_GROUP_H
