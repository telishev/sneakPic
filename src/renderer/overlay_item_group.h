#include "renderable_item.h"
#ifndef OVERLAY_ITEM_GROUP_H
#define OVERLAY_ITEM_GROUP_H

#include "renderer/renderable_item.h"

#include <vector>
#include <memory>

class overlay_item_group : public renderable_item
{
  std::vector<std::unique_ptr<renderable_item>> m_children;
public:
  overlay_item_group ();
  ~overlay_item_group ();

  void add_child (renderable_item *child);

protected:
  virtual void draw (SkCanvas &canvas, const renderer_state &state, const renderer_config *config) const override;
};

#endif // OVERLAY_ITEM_GROUP_H
