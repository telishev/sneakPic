#include "editor/base_anchor_handle.h"

#include <QTransform>

#include "renderer/anchor_handle_renderer.h"
#include "svg/attributes/svg_attribute_nodetypes.h"
#include "svg/items/svg_item_path.h"


base_anchor_handle::base_anchor_handle (svg_path_geom_iterator path_it, svg_item_path *item)
{
  m_path_it = path_it;
  m_node_type = node_type_t::SMOOTH;
  m_item = item;
}

base_anchor_handle::~base_anchor_handle ()
{

}

QPointF base_anchor_handle::get_handle_center () const
{
  QTransform transform = get_path_transform ();
  QPointF point = m_path_it.anchor_point ();
  return transform.map (point);
}

node_type_t base_anchor_handle::node_type () const
{
  return m_node_type;
}

QTransform base_anchor_handle::get_path_transform () const
{
  return m_item ? m_item->full_transform () : QTransform ();
}

void base_anchor_handle::set_path_iterator (svg_path_geom_iterator path_it)
{
  m_path_it = path_it;
}

handle_type base_anchor_handle::get_handle_type () const
{
  return node_type () == node_type_t::CUSP ? handle_type::DIAMOND : handle_type::SQUARE;
}
