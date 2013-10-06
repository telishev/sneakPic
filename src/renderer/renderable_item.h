#ifndef RENDERABLE_ITEM_H
#define RENDERABLE_ITEM_H

class SkCanvas;
class renderer_state;
class renderer_config;

class renderable_item
{
public:
  virtual ~renderable_item () {};

  virtual void draw (SkCanvas &canvas, const renderer_state &state, const renderer_config *config) const = 0;
};

#endif // RENDERABLE_ITEM_H
