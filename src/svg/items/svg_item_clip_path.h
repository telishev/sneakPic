#ifndef SVG_ITEM_CLIP_PATH_H
#define SVG_ITEM_CLIP_PATH_H

#include "svg/items/abstract_svg_item.h"

class QPainterPath;

class svg_item_clip_path : public abstract_svg_item
{
  SVG_ITEM

public:
  svg_item_clip_path (svg_document *document);
  virtual ~svg_item_clip_path () override;

  QPainterPath get_clip_path () const;

protected:
  virtual bool check_item () override;

private:
  QPainterPath get_single_child_path (const abstract_svg_item *child) const;
};

#endif // SVG_ITEM_CLIP_PATH_H
