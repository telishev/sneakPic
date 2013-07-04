#ifndef SVG_ITEM_PATH_H
#define SVG_ITEM_PATH_H

#include "svg/items/svg_base_shape_item.h"

class renderer_item_path;

class svg_item_path : public svg_base_shape_item
{
  SVG_ITEM

  renderer_item_path *m_render_item;
public:
  svg_item_path (svg_document *document);
  virtual ~svg_item_path () override;

  virtual void update_renderer_item ();
  virtual const abstract_renderer_item *get_renderer_item () const;
  virtual bool render_children () const { return false; }

protected:
  virtual bool check_item () override;
  virtual QPainterPath get_path () const override;
};


#endif // SVG_ITEM_PATH_H
