#ifndef CURRENT_ITEM_OUTLINE_RENDERER_H
#define CURRENT_ITEM_OUTLINE_RENDERER_H

#include "renderer/overlay_items_container.h"

class current_item_outline_renderer : public overlay_items_container
{
  string m_item;
public:
  current_item_outline_renderer (svg_items_container *container);
  ~current_item_outline_renderer ();

  void set_current_item (const string &item);
  string current_item () const { return m_item; }

protected:
  virtual renderable_item *create_overlay_item (const string &object) const override;
};

#endif // CURRENT_ITEM_OUTLINE_RENDERER_H
