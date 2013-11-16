#include "renderer/renderer_item_selection.h"

#include <QRectF>
#include <qmath.h>

#include "skia/skia_includes.h"

#include "renderer/qt2skia.h"
#include "renderer/renderer_state.h"

#include "path/geom_helpers.h"


renderer_item_selection::renderer_item_selection ()
{

}

renderer_item_selection::~renderer_item_selection ()
{

}

void renderer_item_selection::draw (SkCanvas &canvas, const renderer_state &state, const renderer_config * /*config*/) const 
{
  canvas.save ();
  canvas.resetMatrix ();

  SkPaint paint;
  paint.setStrokeWidth (0.0);
  paint.setStyle (SkPaint::kStroke_Style);
  paint.setAntiAlias (false);
  paint.setColor (SK_ColorWHITE);
  paint.setXfermodeMode (SkXfermode::kDifference_Mode);
  SkScalar dash_offsets[] = { SkFloatToScalar (4), SkFloatToScalar (4) };
  SkDashPathEffect *effect = new SkDashPathEffect (dash_offsets, 2, 0);
  paint.setPathEffect (effect)->unref ();
  QRect mapped_rect = geom::inner_rect (state.transform ().mapRect (m_bbox));
  canvas.drawRect (qt2skia::rect (mapped_rect), paint);

  canvas.restore ();
}

void renderer_item_selection::set_bbox (const QRectF &rect)
{
  m_bbox = rect;
}
