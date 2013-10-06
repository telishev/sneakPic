#ifndef TRANSFORMED_RENDERER_ITEM_H
#define TRANSFORMED_RENDERER_ITEM_H

#include "renderer/renderable_item.h"

#include <vector>
#include <memory>

#include <QTransform>

class transformed_renderable_items : public renderable_item
{
  std::vector<std::unique_ptr<renderable_item>> m_base_items;
  QTransform m_transform;
public:
  transformed_renderable_items ();
  ~transformed_renderable_items ();

  void add_base_item (renderable_item *base_item);
  void clear_base_items ();
  void set_base_item_transform (const QTransform &transform);


protected:
  virtual void draw (SkCanvas &canvas, const renderer_state &state, const renderer_config *config) const override;

};

#endif // TRANSFORMED_RENDERER_ITEM_H
