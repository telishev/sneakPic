#include "svg/svg_utils.h"

#include "svg/items/svg_item_svg.h"
#include "svg/svg_document.h"




bool svg_utils::get_doc_dimensions (svg_document *doc, double &width, double &height)
{
  const svg_item_svg *svg_item = static_cast <const svg_item_svg *>(doc->root ());
  if (!svg_item)
    return false;

  width = svg_item->width ();
  height = svg_item->height ();
  return true;
}
