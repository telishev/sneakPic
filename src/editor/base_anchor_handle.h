#ifndef BASE_ANCHOR_HANDLE_H
#define BASE_ANCHOR_HANDLE_H

#include "editor/abstract_handle.h"
#include "path/svg_path_geom.h"
#include "svg/attributes/svg_attribute_nodetypes.h"

class svg_item_path;


class base_anchor_handle : public abstract_handle
{
protected:
  svg_path_geom_iterator m_path_it;
  bool m_is_highlighted;
  bool m_is_selected;
  node_type_t m_node_type;
  svg_item_path *m_item;
public:
  base_anchor_handle (svg_path_geom_iterator path_it, svg_item_path *item);
  ~base_anchor_handle ();

  void set_path_iterator (svg_path_geom_iterator path_it);
  virtual QPointF get_handle_center () const;
  void set_node_type (node_type_t node_type) { m_node_type = node_type; }
  void set_is_selected (bool is_selected) { m_is_selected = is_selected; }

  svg_path_geom_iterator path_iterator () const { return m_path_it; }
  svg_item_path *item () const { return m_item; }

protected:
  virtual int distance_to_mouse (QPoint screen_pos, QTransform transform) const override;
  virtual void set_mouse_hovered (bool hovered) override;
  virtual bool start_drag (QPointF local_pos) override;
  virtual bool drag (QPointF local_pos) override;
  virtual bool end_drag (QPointF local_pos) override;
  virtual void draw (SkCanvas &canvas, const renderer_state &state, const renderer_config *config) const override;

  virtual node_type_t node_type () const;
  virtual QRect get_element_rect (QTransform transform) const;
  virtual QTransform get_path_transform () const;
};


#endif // BASE_ANCHOR_HANDLE_H
