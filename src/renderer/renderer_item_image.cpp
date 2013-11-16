#include "renderer/renderer_item_image.h"

#include <QPainter>

#include "renderer/renderer_state.h"
#include "renderer/qt2skia.h"
#include "renderer/renderer_config.h"

#include "svg/attributes/svg_attributes_enum.h"

#include "skia/skia_includes.h"


renderer_item_image::renderer_item_image (const std::string &name)
  : renderer_graphics_item (name)
{
}

renderer_item_image::~renderer_item_image ()
{

}

void renderer_item_image::set_dest_rect (QRectF rect)
{
  m_dst_rect = rect;
}

void renderer_item_image::set_src_rect (QRectF rect)
{
  m_src_rect = rect;
}

void renderer_item_image::set_image_data (QImage &image_data)
{
  m_image_data = image_data;
}

void renderer_item_image::draw_graphics_item (SkCanvas &canvas, const renderer_state &/*state*/, const renderer_config *config) const
{
  SkBitmap bitmap =  qt2skia::image (m_image_data);
  SkPaint paint;

  paint.setAlpha ((int) (m_opacity * 255));
  if (config->render_for_selection ())
  {
    if (!configure_painter_for_selection (paint))
      return;
    canvas.drawRect (qt2skia::rect (m_dst_rect), paint);
  }
  else
    {
      if (!bitmap.isNull ())
      {
        SkIRect irect = qt2skia::Irect (m_src_rect);
        canvas.drawBitmapRect (bitmap, &irect, qt2skia::rect (m_dst_rect), &paint);
      }
    }
}

void renderer_item_image::update_bbox_impl ()
{
  m_bbox_computed = m_bbox;
}
