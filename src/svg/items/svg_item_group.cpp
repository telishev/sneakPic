#include "svg/items/svg_item_group.h"

#include "renderer/renderer_item_group.h"

svg_item_group::svg_item_group (svg_document *document)
  : svg_item_group_type (document)
{
  m_renderer_item = new renderer_item_group (this);
}

svg_item_group::~svg_item_group ()
{

}

void svg_item_group::update_renderer_item ()
{
  update_group_item (m_renderer_item);
}

const abstract_renderer_item * svg_item_group::get_renderer_item () const 
{
  return m_renderer_item;
}



