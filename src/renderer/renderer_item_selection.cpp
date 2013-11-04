#include "renderer/renderer_item_selection.h"

#include <QRectF>
#include <qmath.h>

#pragma warning(push, 0)
#include <SkCanvas.h>
#include <SkSurface.h>
#include <SkDevice.h>
#include <SkDashPathEffect.h>
#pragma warning(pop)

#include "renderer/qt2skia.h"
#include "renderer/renderer_state.h"


renderer_item_selection::renderer_item_selection ()
{

}

renderer_item_selection::~renderer_item_selection ()
{

}

static inline QRect to_rect (QRectF rect)
{
  int left = (int)ceil (rect.left () + 0.5);
  int rigth = (int)floor (rect.right () - 0.5);
  int top = (int)ceil (rect.top () + 0.5);
  int bottom = (int)floor (rect.bottom () - 0.5);
  return QRect (left, top, rigth - left - 1, bottom - top - 1);
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
  QRect mapped_rect = to_rect (state.transform ().mapRect (m_bbox));
  canvas.drawRect (qt2skia::rect (mapped_rect), paint);

  canvas.restore ();
}

void renderer_item_selection::set_bbox (const QRectF &rect)
{
  m_bbox = rect;
}
