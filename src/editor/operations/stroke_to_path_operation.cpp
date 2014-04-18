#include "stroke_to_path_operation.h"

#include <QPainterPath>
#include <QPainterPathStroker>

#include "object_to_path_operation.h"
#include "editor/item_paint_style.h"

#include "svg/items/abstract_svg_item.h"
#include "svg/items/svg_item_path.h"

#include "renderer/renderer_paint_server.h"

#include "path/qt_path_conversion.h"
#include "path_edit_operation.h"


void stroke_to_path_operation::apply (abstract_svg_item *item)
{
  svg_item_path *path_item = object_to_path_operation ().apply (item);
  if (!path_item)
    return;

  QPainterPath path = path_item->get_path ();
  item_paint_style style;
  style.create_from_item (path_item);
  QPainterPathStroker stroker;
  const stroke_config &cfg = style.stroke_cfg ();
  stroker.setCapStyle (cfg.linecap ());
  stroker.setJoinStyle (cfg.linejoin ());
  stroker.setMiterLimit (cfg.miterlimit ());
  stroker.setWidth (cfg.width ());

  QPainterPath stroke_path = stroker.createStroke (path);

  item_paint_server empty_stroke;
  empty_stroke.set_current_type (renderer_paint_server_type::NONE);
  style.set_fill (style.stroke ());
  style.set_stroke (empty_stroke);
  style.apply_to_item (path_item);
  path_edit_operation path_edit (path_item);
  qt_path_conversion ().create_from_qpath (stroke_path, *path_edit.get_svg_path ());
}
