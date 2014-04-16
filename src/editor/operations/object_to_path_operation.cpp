#include "object_to_path_operation.h"

#include <QPainterPath>

#include "editor/item_paint_style.h"
#include "editor/operations/path_edit_operation.h"

#include "svg/items/abstract_svg_item.h"
#include "svg/items/svg_graphics_item.h"
#include "svg/items/svg_item_type.h"
#include "svg/items/svg_item_path.h"
#include "svg/svg_document.h"
#include "svg/undo/undo_handler.h"
#include "svg/attributes/svg_attribute_id.h"
#include "svg/attributes/svg_attribute_path_data.h"

#include "path/qt_path_conversion.h"


void object_to_path_operation::apply (abstract_svg_item *item)
{
  svg_base_shape_item *graph_item = dynamic_cast<svg_base_shape_item *> (item);
  if (!graph_item || graph_item->type () == svg_item_type::PATH)
    return;
  
  item_paint_style style;
  style.create_from_item (item);

  QPainterPath painter_path = graph_item->get_path ();

  svg_document *document = item->document ();
  auto path = new svg_item_path (document);
  path->replace_item (item);
  style.apply_to_item (path);
  path_edit_operation op (path);
  qt_path_conversion ().create_from_qpath (painter_path, *op.get_svg_path ());
}

