#ifndef SVG_ITEM_MARKER_H
#define SVG_ITEM_MARKER_H
#include "svg/items/abstract_svg_item.h"

class QPainterPath;
class QPointF;
class QTransform;
class SkCanvas;
class renderer_config;
class renderer_state;

class svg_item_marker : public abstract_svg_item
{
  SVG_ITEM

public:
  svg_item_marker (svg_document *document);
  virtual ~svg_item_marker () override;
  abstract_renderer_item *create_renderer_item_for_marker (QPointF target_point, QPointF bisector, QTransform transform, double stroke_width);

protected:
  virtual bool check_item () override;

};


#endif // SVG_ITEM_MARKER_H
