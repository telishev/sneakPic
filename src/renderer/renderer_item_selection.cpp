#include "renderer/renderer_item_selection.h"

#include <QRectF>

#pragma warning(push, 0)
#include <SkCanvas.h>
#include <SkSurface.h>
#include <SkDevice.h>
#include <SkDashPathEffect.h>
#pragma warning(pop)

#include "renderer/qt2skia.h"
#include "renderer/renderer_state.h"


renderer_item_selection::renderer_item_selection (const std::string &name)
  : abstract_renderer_item (name)
{

}

renderer_item_selection::~renderer_item_selection ()
{

}

void renderer_item_selection::draw (SkCanvas &canvas, const renderer_state &state, const renderer_config * /*config*/) const 
{
  canvas.save ();
  canvas.setMatrix (qt2skia::matrix (state.transform ()));

  SkPaint paint;
  double stroke_width = 2.0 / qMax (state.transform ().m11 (), state.transform ().m22 ());
  paint.setStrokeWidth (stroke_width);
  paint.setStyle (SkPaint::kStroke_Style);
  paint.setAntiAlias (true);
  SkScalar dash_offsets[] = { SkFloatToScalar (2 * stroke_width), SkFloatToScalar (stroke_width) };
  SkDashPathEffect *effect = new SkDashPathEffect (dash_offsets, 2, 0);
  paint.setPathEffect (effect)->unref ();
  canvas.drawRect (qt2skia::rect (m_bbox), paint);

  canvas.restore ();
}

void renderer_item_selection::update_bbox ()
{

}

void renderer_item_selection::set_bbox (const QRectF &rect)
{
  m_bbox = rect;
}
