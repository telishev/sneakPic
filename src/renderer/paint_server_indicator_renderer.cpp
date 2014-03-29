#include "paint_server_indicator_renderer.h"

#include <QImage>

#include "skia/skia_includes.h"
#include "renderer/qt2skia.h"
#include "renderer_paint_server.h"


paint_server_indicator_renderer::paint_server_indicator_renderer (bool is_stroke, double stroke_width)
{
  m_is_stroke = is_stroke;
  m_stroke_width = stroke_width;
}

paint_server_indicator_renderer::~paint_server_indicator_renderer ()
{

}

QImage paint_server_indicator_renderer::create_image (const renderer_paint_server *server, QSize size)
{
  SkBitmap bitmap;
  bitmap.setConfig (SkBitmap::kARGB_8888_Config, size.width (), size.height ());
  bitmap.allocPixels ();
  bitmap.eraseColor (SK_ColorTRANSPARENT);

  SkBitmapDevice device (bitmap);
  SkCanvas canvas (&device);

  QRectF rect (0, 0, size.width (), size.height ());
  SkPaint paint;
  server->fill_paint (paint, rect);

  paint.setStyle (m_is_stroke ? SkPaint::kStroke_Style : SkPaint::kFill_Style);

  if (m_is_stroke)
    paint.setStrokeWidth (m_stroke_width);

  canvas.drawRect (qt2skia::rect (rect), paint);

  if (server->type () == renderer_paint_server_type::NONE)
    {
      SkPaint cur_fill;
      cur_fill.setStyle (SkPaint::kFill_Style);
      cur_fill.setColor (qt2skia::color ({"white"}));
      canvas.drawRect (qt2skia::rect (rect), cur_fill);

      SkPaint cur_stroke;
      cur_stroke.setStyle (SkPaint::kStroke_Style);
      cur_stroke.setStrokeWidth (4);
      cur_stroke.setColor (qt2skia::color ({"red"}));
      canvas.drawLine (rect.right (), rect.top (), rect.left (), rect.bottom (), cur_stroke);
    }
  
  return qt2skia::qimage (bitmap).copy ();
}
