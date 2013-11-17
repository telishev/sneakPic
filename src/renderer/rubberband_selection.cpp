#include "renderer/rubberband_selection.h"

#include <QRectF>
#include "skia/skia_includes.h"

#include "editor/items_selection.h"

#include "common/memory_deallocation.h"

#include "renderer/abstract_renderer_item.h"
#include "renderer/renderer_state.h"
#include "renderer/qt2skia.h"
#include "renderer/renderer_config.h"
#include "renderer/overlay_renderer.h"
#include "renderer/svg_painter.h"

#include "gui/actions_applier.h"
#include "gui/shortcuts_config.h"
#include "gui/mouse_filter.h"

#include "svg/items/svg_items_container.h"


class rubberband_renderer_item : public renderable_item
{
  QRectF m_rect;
public:
  rubberband_renderer_item () {}
  ~rubberband_renderer_item () {}

  void set_rect (const QRectF &val) { m_rect = val; }

  virtual void draw (SkCanvas &canvas, const renderer_state &state, const renderer_config *config) const override
  {
    canvas.setMatrix (qt2skia::matrix (state.transform ()));

    if (config->render_for_selection ())
      return;

    {
      SkPaint paint;
      paint.setStyle (SkPaint::kStroke_Style);
      paint.setStrokeWidth (0);
      canvas.drawRect (qt2skia::rect (m_rect), paint);
    }
  }
};

rubberband_selection::rubberband_selection (overlay_renderer *overlay, svg_painter *painter, actions_applier *applier, mouse_drag_shortcut_enum drag_shortcut)
{
  m_start_x = 0.0;
  m_start_y = 0.0;
  m_end_x = 0.0;
  m_end_y = 0.0;
  m_render_item = new rubberband_renderer_item;
  overlay->add_item (m_render_item, overlay_layer_type::TEMP);  m_painter = painter;
  m_applier = applier;

  m_applier->add_drag_shortcut (drag_shortcut, this,
    &rubberband_selection::start_selection, &rubberband_selection::move_selection, &rubberband_selection::end_selection);
}

rubberband_selection::~rubberband_selection ()
{
  FREE (m_render_item);
}

bool rubberband_selection::start_selection (const mouse_event_t &m_event)
{
  if (m_start_func && !m_start_func (m_event))
    return false;

  QPointF start_point = m_painter->get_local_pos (m_event.pos ());
  m_end_x = m_start_x = start_point.x ();
  m_end_y = m_start_y = start_point.y ();
  m_render_item->set_rect (selection_rect ());
  m_painter->redraw ();
  return true;
}

bool rubberband_selection::move_selection (const mouse_event_t &m_event)
{
  if (m_drag_func && !m_drag_func (m_event))
    return false;

  QPointF start_point = m_painter->get_local_pos (m_event.pos ());
  m_end_x = start_point.x ();
  m_end_y = start_point.y ();
  m_render_item->set_rect (selection_rect ());
  m_painter->redraw ();
  return true;
}

bool rubberband_selection::end_selection (const mouse_event_t &event)
{
  if (m_end_func && !m_end_func (event))
    return false;

  m_start_x = 0.0;
  m_start_y = 0.0;
  m_end_x = 0.0;
  m_end_y = 0.0;
  m_render_item->set_rect (selection_rect ());

  m_painter->redraw ();
  return true;
}

QRectF rubberband_selection::selection_rect () const
{
  return QRectF (qMin (m_start_x, m_end_x), qMin (m_start_y, m_end_y), fabs (m_end_x - m_start_x), fabs (m_end_y - m_start_y));
}
