#ifndef ABSTRACT_RENDERER_ITEM_H
#define ABSTRACT_RENDERER_ITEM_H

class QPainter;
class renderer_model;

class abstract_renderer_item
{
public:
  virtual ~abstract_renderer_item () {};

  virtual void draw (QPainter &painter) const = 0;
};

#endif // ABSTRACT_RENDERER_ITEM_H
