#ifndef TRANSFORMED_RENDERER_ITEM_H
#define TRANSFORMED_RENDERER_ITEM_H

#include "renderer/abstract_renderer_item.h"

#include <vector>
#include <memory>

#include <QTransform>

class transformed_renderer_items : public abstract_renderer_item
{
  std::vector<std::unique_ptr<abstract_renderer_item>> m_base_items;
  QTransform m_transform;
public:
  transformed_renderer_items ();
  ~transformed_renderer_items ();

  void add_base_item (abstract_renderer_item *base_item);
  void clear_base_items ();
  void set_base_item_transform (const QTransform &transform);


protected:
  virtual void draw (SkCanvas &canvas, const renderer_state &state, const renderer_config *config) const override;
  virtual void update_bbox_impl () override;
  virtual QRectF bounding_box_impl () const override;

};

#endif // TRANSFORMED_RENDERER_ITEM_H
