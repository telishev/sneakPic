#include "svg/attributes/svg_attribute_nodetypes.h"

#include "common/debug_utils.h"

#include "path/svg_path_geom.h"
#include "path/geom_helpers.h"


svg_attribute_nodetypes::svg_attribute_nodetypes ()
{

}

svg_attribute_nodetypes::~svg_attribute_nodetypes ()
{

}

bool svg_attribute_nodetypes::read (const char *data, bool /*from_css*/)
{
  m_node_type.clear ();
  for (const char *cur_data = data; *cur_data; cur_data++)
    {
      node_type_t type = char_to_node_type (*cur_data);
      if (type == node_type_t::COUNT)
        return false;

      m_node_type.push_back (type);
    }

  return true;
}

bool svg_attribute_nodetypes::write (QString &data, bool /*to_css*/) const
{
  for (size_t i = 0; i < m_node_type.size (); i++)
    data += node_type_to_char (m_node_type[i]);

  return true;
}

char svg_attribute_nodetypes::node_type_to_char (node_type_t type) const
{
  switch (type)
    {
    case node_type_t::CUSP: return 'c';
    case node_type_t::SMOOTH: return 's';
    case node_type_t::SYMMETRIC: return 'z';

    case node_type_t::COUNT: break;
    }

  DEBUG_PAUSE ("wrong type");
  return 'c';
}

node_type_t svg_attribute_nodetypes::char_to_node_type (char val) const
{
  for (int i = 0; i < (int)node_type_t::COUNT; i++)
    if (val == node_type_to_char ((node_type_t)i))
      return (node_type_t)i;

  return node_type_t::COUNT;
}

void svg_attribute_nodetypes::create_from_path (const svg_path_geom *path, bool dont_create_if_exists)
{
  if (dont_create_if_exists && m_node_type.size () == path->total_points ())
    return;

  m_node_type.clear ();
  m_node_type.resize (path->total_points (), node_type_t::CUSP);
  size_t cur_point = 0;

  for (auto it = path->begin (); it != path->end (); ++it, cur_point++)
    {
      if (   !it.has_control_point (cp_type::LEFT)
          || !it.has_control_point (cp_type::RIGHT))
        continue;

      QPointF left_cp = it.control_point (cp_type::LEFT);
      QPointF right_cp = it.control_point (cp_type::RIGHT);
      QPointF anchor = it.anchor_point ();

      if (!geom::are_line (anchor, left_cp, right_cp))
        continue;

      if (right_cp - anchor == anchor - left_cp)
        m_node_type[cur_point] = node_type_t::SYMMETRIC;
      else
        m_node_type[cur_point] = node_type_t::SMOOTH;
    }
}
