#ifndef BASE_HANDLE_H
#define BASE_HANDLE_H

#include "abstract_handle.h"
#include "renderer/anchor_handle_renderer.h"

enum class handle_type;
class QPointF;
class QRect;

class base_handle : public abstract_handle
{
  handle_type m_handle_type;
  mutable anchor_handle_renderer m_renderer;
public:
  base_handle ();
  ~base_handle ();

  void set_highlighted (bool highlighted);
  void set_selected (bool selected);
  void set_handle_type (handle_type type) { m_handle_type = type; }
  void set_color (QColor color);
  void set_selected_color (QColor color);
  void set_highlighted_color (QColor color);

  virtual int distance_to_mouse (QPoint screen_pos, QTransform transform) const override;
  virtual void set_mouse_hovered (bool hovered) override;
  virtual bool start_drag (QPointF local_pos, QTransform transform) override;
  virtual bool drag (QPointF local_pos, QTransform transform, keyboard_modifier modifier) override;
  virtual bool end_drag (QPointF local_pos, QTransform transform, keyboard_modifier modifier) override;
  virtual void interrupt_drag () override {};
  virtual void draw (SkCanvas &canvas, const renderer_state &state, const renderer_config *config) const override;
  virtual handle_type get_handle_type () const;
  virtual int get_handle_size () const;
  virtual float handle_rotation () const;
  virtual QRect get_element_rect (QTransform transform) const;
  virtual QPointF get_handle_center () const = 0;
  virtual bool is_visible () const { return true; };
};

#endif // BASE_HANDLE_H
