#include "renderer/renderer_item_path.h"

#include <QPainter>

#include "renderer/renderer_state.h"
#include "renderer/qt2skia.h"

#pragma warning(push, 0)
#include <SkCanvas.h>
#include <SkSurface.h>
#include <SkDevice.h>
#include <SkPoint.h>
#pragma warning(pop)


renderer_item_path::renderer_item_path ()
{
}

renderer_item_path::~renderer_item_path ()
{

}


void renderer_item_path::draw (SkCanvas &canvas, const renderer_state &state) const
{
  QTransform item_transform = transform () * state.transform ();
  QRectF transformed_rect = state.transform ().mapRect (bounding_box ());
  if (!state.rect ().intersects (transformed_rect.toRect ()))
    return;

  SkPath path = qt2skia::path (m_path);
  canvas.save ();
  canvas.setMatrix (qt2skia::matrix (item_transform));
  if (m_has_clip_path)
    canvas.clipPath (qt2skia::path (m_clip_path), SkRegion::kReplace_Op, true);

  for (int i = 0; i < 2; i++)
    {
      SkPaint paint;
      if (!configure_painter (paint, i != 0))
        continue;

      canvas.drawPath (path, paint);
    }

  canvas.restore ();
}

void renderer_item_path::set_painter_path (const QPainterPath &path)
{
  m_path = path;
  m_bbox = m_path.controlPointRect ();
  adjust_bbox (m_bbox);
}
