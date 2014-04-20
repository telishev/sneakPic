#ifndef ANCHOR_HANDLE_RENDERER_H
#define ANCHOR_HANDLE_RENDERER_H

#include "renderer/renderable_item.h"

#include <QPointF>
#include <QColor>

enum class handle_type
{
  SQUARE,
  DIAMOND,
  CIRCLE,
  DOUBLE_HEADED_ARROW,
};

class QTransform;
struct SkRect;
class SkPaint;

class anchor_handle_renderer : public renderable_item
{
  int m_anchor_size_px = 7;
  QPointF m_pos;
  handle_type m_node_type;
  float m_rotation_angle; // applied only to non symetrical figures
  bool m_is_highlighted;
  bool m_is_visible = false;
  bool m_is_selected = false;
  QColor m_highlighted_color;
  QColor m_selected_color;
  QColor m_color;

public:
  anchor_handle_renderer ();
  anchor_handle_renderer (QPointF pos, handle_type node_type, bool is_highlighted);
  ~anchor_handle_renderer ();

  void set_color (QColor color) {m_color = color;}
  void set_selected_color (QColor color) {m_selected_color = color;}
  void set_highlighted_color (QColor color) {m_highlighted_color = color;}
  void set_pos (QPointF pos);
  void set_node_type (handle_type node_type);
  void set_rotation (float angle);
  void set_highlighted (bool is_highlighted);
  void set_visible (bool visible);
  void set_is_selected (bool is_selected);
  void set_anchor_size (int px) { m_anchor_size_px = px;};
  virtual void draw (SkCanvas &canvas, const renderer_state &state, const renderer_config *config) const override;

  int get_anchor_size_px () { return m_anchor_size_px; }
private:
  void draw_anchor (SkCanvas &canvas, const SkRect &rect, SkPaint &paint) const;
  QRect get_element_rect (QTransform transform) const;
  QColor current_color () const;
};

#endif // ANCHOR_HANDLE_RENDERER_H
