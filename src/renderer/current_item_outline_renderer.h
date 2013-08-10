#ifndef CURRENT_ITEM_OUTLINE_RENDERER_H
#define CURRENT_ITEM_OUTLINE_RENDERER_H

#include "renderer/overlay_items_container.h"

class current_item_outline_renderer : public overlay_items_container
{
  std::string m_item;
public:
  current_item_outline_renderer (overlay_renderer *overlay);
  ~current_item_outline_renderer ();

  void set_current_item (const std::string &item);
  std::string current_item () const { return m_item; }

protected:
  virtual std::vector<abstract_renderer_item *> create_overlay_item (const std::string &object) const override;
};

#endif // CURRENT_ITEM_OUTLINE_RENDERER_H
