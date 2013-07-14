#include "renderer/renderer_item_image.h"

#include <QPainter>

#include "renderer/renderer_state.h"
#include "renderer/qt2skia.h"
#include "renderer/renderer_config.h"

#include "svg/attributes/svg_attributes_enum.h"

#pragma warning(push, 0)
#include <SkCanvas.h>
#include <SkSurface.h>
#include <SkDevice.h>
#include <SkPoint.h>
#pragma warning(pop)


renderer_item_image::renderer_item_image (const std::string &name)
  : abstract_renderer_item (name)
{
}

renderer_item_image::~renderer_item_image ()
{

}

void renderer_item_image::set_dimensions (double x, double y, double w, double h)
{
  m_x = x;
  m_y = y;
  m_width = w;
  m_height =h;
}

void renderer_item_image::set_image_data (QImage &image_data)
{
  m_image_data = image_data;
}

void renderer_item_image::draw (SkCanvas &canvas, const renderer_state &state, const renderer_config *config) const
{
  if (m_display == display::NONE)
    return;

  QTransform item_transform = transform () * state.transform ();
  QRectF transformed_rect = state.transform ().mapRect (bounding_box ());
  if (!state.rect ().intersects (transformed_rect.toRect ()))
    return;

  SkBitmap bitmap =  qt2skia::image (m_image_data);
  canvas.save ();
  canvas.setMatrix (qt2skia::matrix (item_transform));
  SkPaint paint;
  
  if (config->render_for_selection ())
  {
    if (!configure_painter_for_selection (paint))
      return;
    canvas.drawRect (SkRect::MakeXYWH (m_x, m_y, m_width, m_height), paint);
    // TODO: probably support holes in selection if opacity is 0 (check if this is right behaviour)
  }
  else
    canvas.drawBitmapRect (bitmap, SkRect::MakeXYWH (m_x, m_y, m_width, m_height), &paint);

  canvas.restore ();
}

void renderer_item_image::update_bbox ()
{
  m_bbox = QRect (m_x, m_y, m_width, m_height);
}
