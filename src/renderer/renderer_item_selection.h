#ifndef RENDERER_ITEM_SELECTION_H
#define RENDERER_ITEM_SELECTION_H

#include "renderer/renderable_item.h"

#include <QRectF>

class renderer_item_selection : public renderable_item
{
  QRectF m_bbox;
public:
  renderer_item_selection ();
  ~renderer_item_selection ();

  virtual void draw (SkCanvas &canvas, const renderer_state &state, const renderer_config *config) const override;

  void set_bbox (const QRectF &rect);
};

#endif // RENDERER_ITEM_SELECTION_H
