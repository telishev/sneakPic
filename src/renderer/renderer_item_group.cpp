#include "renderer/renderer_item_group.h"

#include "svg/attributes/svg_attributes_enum.h"

#include "svg/items/abstract_svg_item.h"

#include "renderer/renderer_state.h"
#include "renderer/qt2skia.h"
#include "renderer/renderer_items_container.h"
#include "renderer/renderer_config.h"

#include <QPixmap>
#include <QPainter>

#pragma warning(push, 0)
#include <SkCanvas.h>
#include <SkSurface.h>
#include <SkDevice.h>
#pragma warning(pop)

renderer_item_group::renderer_item_group (const std::string &name)
  : abstract_renderer_item (name)
{
  m_opacity = 1.0;
  m_has_clip_path = false;
}

renderer_item_group::~renderer_item_group ()
{

}

void renderer_item_group::draw (SkCanvas &canvas, const renderer_state &state, const renderer_config *config) const
{
  if ((config && config->is_interrupted ()) || m_display == display::NONE)
    return;

  QTransform item_transform = transform () * state.transform ();
  QRectF transformed_rect = state.transform ().mapRect (bounding_box ());
  QRect result_rect = state.rect ().intersected (transformed_rect.toRect ());
  if (result_rect.isNull ())
    return;

  SkBitmap bitmap;
  bitmap.setConfig (SkBitmap::kARGB_8888_Config, result_rect.width (), result_rect.height ());
  bitmap.allocPixels ();
  SkDevice device(bitmap);
  SkCanvas child_canvas (&device);
  SkPaint paint;

  child_canvas.drawColor (SK_ColorTRANSPARENT, SkXfermode::kSrc_Mode);

  QTransform pixmap_transform = QTransform::fromTranslate (state.rect ().x () - result_rect.x (), state.rect ().y () - result_rect.y ());

  renderer_state new_state (state.rect (), item_transform * pixmap_transform);

  for (const std::string &child_name : m_children)
    {
      abstract_renderer_item *child_item = container ()->item (child_name);
      if (child_item)
        child_item->draw (child_canvas, new_state, config);

      if (config && config->is_interrupted ())
        return;
    }

  canvas.save ();
  if (m_has_clip_path)
    {
      QPainterPath clip_path = item_transform.map (m_clip_path);
      canvas.clipPath (qt2skia::path (clip_path), SkRegion::kReplace_Op, !config->render_for_selection ());
    }

  canvas.resetMatrix ();
  if (!config->render_for_selection ())
    paint.setAlpha (m_opacity * 255);
  canvas.drawBitmap (bitmap, result_rect.x (), result_rect.y (), &paint);
  canvas.restore ();
  return;
}

QRectF renderer_item_group::bounding_box () const 
{
  return m_bbox;
}

void renderer_item_group::update_bbox ()
{
  QRectF new_box;
  for (const std::string &child_name : m_children)
    {
      abstract_renderer_item *renderer_item = container ()->item (child_name);
      if (!renderer_item)
        continue;

      renderer_item->update_bbox ();
      new_box = new_box.united (renderer_item->bounding_box ());
    }

  m_bbox = transform ().mapRect (new_box);
}
