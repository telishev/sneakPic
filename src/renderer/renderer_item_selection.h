#ifndef RENDERER_ITEM_SELECTION_H
#define RENDERER_ITEM_SELECTION_H

#include "renderer/abstract_renderer_item.h"

class renderer_item_selection : public abstract_renderer_item
{
  QRectF m_bbox;
public:
  renderer_item_selection (const std::string &name);
  ~renderer_item_selection ();

  virtual void draw (SkCanvas &canvas, const renderer_state &state, const renderer_config *config) const override;
  virtual QRectF bounding_box_impl () const override { return m_bbox; }
  virtual void update_bbox_impl () override;

  void set_bbox (const QRectF &rect);
};

#endif // RENDERER_ITEM_SELECTION_H
