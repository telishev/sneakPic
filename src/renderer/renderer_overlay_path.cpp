#include "renderer/renderer_overlay_path.h"

#pragma warning(push, 0)
#include <SkCanvas.h>
#include <SkSurface.h>
#include <SkDevice.h>
#pragma warning(pop)

#include <QColor>

#include "renderer/renderer_state.h"
#include "renderer/qt2skia.h"



renderer_overlay_path::renderer_overlay_path (const std::string &name)
  : abstract_renderer_item (name)
{

}

renderer_overlay_path::~renderer_overlay_path ()
{

}

void renderer_overlay_path::draw (SkCanvas &canvas, const renderer_state &state, const renderer_config * /*config*/) const 
{
  SkPath path = qt2skia::path (m_path);
  canvas.save ();
  canvas.setMatrix (qt2skia::matrix (state.transform ()));

  SkPaint paint;
  paint.setColor (qt2skia::color (QColor ("slateblue")));
  double stroke_width = 2.0 / qMax (state.transform ().m11 (), state.transform ().m22 ());
  paint.setStrokeWidth (stroke_width);
  paint.setStyle (SkPaint::kStroke_Style);
  paint.setAntiAlias (true);
  canvas.drawPath (path, paint);

  canvas.restore ();
}

void renderer_overlay_path::update_bbox ()
{
  m_bbox = m_path.controlPointRect ();
  double adjust_value = 1.0;
  m_bbox.adjust (-adjust_value, -adjust_value, adjust_value, adjust_value);
}

void renderer_overlay_path::set_painter_path (const QPainterPath &path)
{
  m_path = path;
}
