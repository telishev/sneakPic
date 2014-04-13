#ifndef GRADIENT_HANDLES_EDITOR_H
#define GRADIENT_HANDLES_EDITOR_H

#include "editor/selection_handles_editor.h"
#include "element_handles.h"
#include "item_paint_server.h"

class base_gradient_handle;
class gradient_handles_color_model;

class gradient_handles : public element_handles
{
  abstract_svg_item *m_item;
  item_paint_server m_server;
  std::vector<abstract_handle *> m_handles;
  bool m_is_fill;
public:
  gradient_handles (abstract_svg_item *item, bool is_fill);
  ~gradient_handles ();

  void apply_changes ();
  abstract_svg_item *item () const;
  virtual vector<abstract_handle *> handles () override;
  double distance_to_line (QPointF pos, QTransform cur_transform) const;
  void add_handle (QPointF pos, QTransform cur_transform);
};

class gradient_handles_editor : public selection_handles_editor
{
  unique_ptr<gradient_handles_color_model> m_color_model;

  std::pair<std::string, int> m_selected_handle;
  bool m_disable_deselect;
public:
  gradient_handles_editor (overlay_renderer *overlay, svg_painter *painter, actions_applier *applier);
  virtual ~gradient_handles_editor ();

  std::pair<std::string, int> selected_handle () const { return m_selected_handle; }

  void set_page_active (bool is_active);
  base_gradient_handle *current_selection () const;

protected:
  virtual element_handles *create_handles_for_item (abstract_svg_item *item) override;
  virtual void update_handles_impl () override;

private:
  bool select_handle (const QPointF &pos);
  bool deselect_handles ();
  bool remove_handles ();
  void set_selected_handle (std::pair<std::string, int> handle);
  bool add_gradient_stop (const QPointF &pos);
};

#endif // GRADIENT_HANDLES_EDITOR_H
