#ifndef SVG_ITEM_LINE_H
#define SVG_ITEM_LINE_H

#include "svg/items/svg_base_shape_item.h"

class renderer_item_line;

class svg_item_line : public svg_base_shape_item
{
  SVG_ITEM

  renderer_item_line *m_render_item;
public:
  svg_item_line (svg_document *document);
  virtual ~svg_item_line () override;

  virtual void update_renderer_item ();
  virtual const abstract_renderer_item *get_renderer_item () const;

protected:
  virtual bool check_item () override;

};


#endif // SVG_ITEM_LINE_H
