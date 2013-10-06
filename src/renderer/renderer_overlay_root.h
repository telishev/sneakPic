#ifndef RENDERER_OVERLAY_ROOT_H
#define RENDERER_OVERLAY_ROOT_H

#include "renderer/renderable_item.h"

#include <set>

class renderer_overlay_root : public renderable_item
{
  std::set<renderable_item *> m_items;

public:
  renderer_overlay_root ();
  ~renderer_overlay_root ();

  void add_item (renderable_item *item);
  void remove_item (renderable_item *item);

  bool empty () const;

protected:
  virtual void draw (SkCanvas &canvas, const renderer_state &state, const renderer_config *config) const override;
};

#endif // RENDERER_OVERLAY_ROOT_H
