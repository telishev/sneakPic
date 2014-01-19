#ifndef ANCHOR_HANDLE_RENDERER_H
#define ANCHOR_HANDLE_RENDERER_H

#include "renderer/renderable_item.h"

#include <QPointF>
enum class node_type_t : char;

class QTransform;
struct SkRect;
class SkPaint;

class anchor_handle_renderer : public renderable_item
{
  static const int anchor_size_px = 7;
  QPointF m_pos;
  node_type_t m_node_type;
  bool m_is_highlighted;
  bool m_is_visible;
public:
  anchor_handle_renderer ();
  anchor_handle_renderer (QPointF pos, node_type_t node_type, bool is_highlighted);
  ~anchor_handle_renderer ();

  void set_pos (QPointF pos);
  void set_node_type (node_type_t node_type);
  void set_highlighted (bool is_highlighted);
  void set_visible (bool visible);
  virtual void draw (SkCanvas &canvas, const renderer_state &state, const renderer_config *config) const override;

  static int get_anchor_size_px () { return anchor_size_px; }
private:
  void draw_anchor (SkCanvas &canvas, const SkRect &rect, SkPaint &paint) const;
  bool is_cusp_node () const;
  QRect get_element_rect (QTransform transform) const;
  QColor current_color () const;
};

#endif // ANCHOR_HANDLE_RENDERER_H
