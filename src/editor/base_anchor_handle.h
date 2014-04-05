#ifndef BASE_ANCHOR_HANDLE_H
#define BASE_ANCHOR_HANDLE_H

#include "path/svg_path_geom.h"
#include "svg/attributes/svg_attribute_nodetypes.h"
#include "editor/base_handle.h"

class svg_item_path;

class base_anchor_handle : public base_handle
{
protected:
  svg_path_geom_iterator m_path_it;
  node_type_t m_node_type;
  svg_item_path *m_item;
public:
  base_anchor_handle (svg_path_geom_iterator path_it, svg_item_path *item);
  ~base_anchor_handle ();

  void set_path_iterator (svg_path_geom_iterator path_it);
  virtual QPointF get_handle_center () const override;
  void set_node_type (node_type_t node_type) { m_node_type = node_type; }

  svg_path_geom_iterator path_iterator () const { return m_path_it; }
  svg_item_path *item () const { return m_item; }

protected:
  virtual node_type_t node_type () const;
  virtual QTransform get_path_transform () const;
  virtual handle_type get_handle_type () const override;
};


#endif // BASE_ANCHOR_HANDLE_H

