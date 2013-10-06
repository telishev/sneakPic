#include "renderer/rubberband_selection.h"

#include <QRectF>
#pragma warning(push, 0)
#include <SkCanvas.h>
#pragma warning(pop)

#include "renderer/abstract_renderer_item.h"
#include "renderer/renderer_state.h"
#include "renderer/qt2skia.h"
#include "renderer/renderer_config.h"
#include "renderer/overlay_renderer.h"


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

rubberband_selection::rubberband_selection (overlay_renderer *overlay)
{
  m_start_x = 0.0;
  m_start_y = 0.0;
  m_end_x = 0.0;
  m_end_y = 0.0;
  m_render_item = new rubberband_renderer_item;
  overlay->add_item (m_render_item, overlay_layer_type::TEMP);
}

rubberband_selection::~rubberband_selection ()
{

}

void rubberband_selection::start_selection (double pos_x, double pos_y)
{
  m_end_x = m_start_x = pos_x;
  m_end_y = m_start_y = pos_y;
  m_render_item->set_rect (selection_rect ());
}

void rubberband_selection::move_selection (double pos_x, double pos_y)
{
  m_end_x = pos_x;
  m_end_y = pos_y;
  m_render_item->set_rect (selection_rect ());
}

void rubberband_selection::end_selection ()
{
  m_start_x = 0.0;
  m_start_y = 0.0;
  m_end_x = 0.0;
  m_end_y = 0.0;
  m_render_item->set_rect (selection_rect ());
}

QRectF rubberband_selection::selection_rect () const
{
  return QRectF (qMin (m_start_x, m_end_x), qMin (m_start_y, m_end_y), fabs (m_end_x - m_start_x), fabs (m_end_y - m_start_y));
}
