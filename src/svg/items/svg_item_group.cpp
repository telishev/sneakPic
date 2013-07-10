#include "svg/items/svg_item_group.h"

#include "renderer/renderer_item_group.h"

svg_item_group::svg_item_group (svg_document *document)
  : svg_item_group_type (document)
{
  
}

svg_item_group::~svg_item_group ()
{

}

abstract_renderer_item *svg_item_group::create_renderer_item () const 
{
  renderer_item_group *renderer_item = new renderer_item_group (id ().toStdString ());
  update_group_item (renderer_item);
  return renderer_item;
}



