#ifndef GRADIENT_HANDLES_EDITOR_H
#define GRADIENT_HANDLES_EDITOR_H

#include "editor/handles_editor.h"

class base_gradient_handle;
class gradient_handles_color_model;

class gradient_handles_editor : public handles_editor
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
  void set_selected_handle (std::pair<std::string, int> handle);
};

#endif // GRADIENT_HANDLES_EDITOR_H
