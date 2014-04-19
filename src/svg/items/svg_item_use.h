#ifndef SVG_ITEM_USE_H
#define SVG_ITEM_USE_H

#include "svg/items/svg_item_group_type.h"

class use_item_watcher;
class QTransform;

class svg_item_use : public svg_item_group_type
{
  SVG_ITEM

public:
  svg_item_use (svg_document *document);
  virtual ~svg_item_use () override;

  bool update_children_tree ();
  virtual abstract_renderer_item *create_renderer_item_impl () const override;

  void unlink ();

protected:
  virtual bool process_item_after_read () override;
  virtual bool can_be_selected () const override;
  void unlink_item (abstract_svg_item *item, QTransform transform);

};


#endif // SVG_ITEM_USE_H
