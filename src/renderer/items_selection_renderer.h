#include "overlay_items_container.h"
#ifndef ITEMS_SELECTION_RENDERER_H
#define ITEMS_SELECTION_RENDERER_H

#include "renderer/overlay_items_container.h"

class items_selection;

class items_selection_renderer : public overlay_items_container
{
  items_selection *m_selection;

public:
  items_selection_renderer (svg_items_container *container, items_selection *selection);
  ~items_selection_renderer ();

  virtual void update_items () override;

protected:
  virtual renderable_item *create_overlay_item (const string &object) const override;
};

#endif // ITEMS_SELECTION_RENDERER_H
