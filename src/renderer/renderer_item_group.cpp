#include "renderer/renderer_item_group.h"

#include "svg/attributes/svg_attributes_enum.h"

#include "renderer/renderer_state.h"
#include "renderer/qt2skia.h"
#include "renderer/renderer_items_container.h"
#include "renderer/renderer_config.h"

#include <QPixmap>
#include <QPainter>

#include "skia/skia_includes.h"

renderer_item_group::renderer_item_group (const string &name)
  : abstract_renderer_item (name)
{
  m_opacity = 1.0;
  m_has_clip_path = false;
}

renderer_item_group::~renderer_item_group ()
{

}

static inline QRect bounding_rect (const QRectF &rect)
{
  int left = floor (rect.left ()), right = ceil (rect.right ());
  int top = floor (rect.top ()), bottom = ceil (rect.bottom ());
  return QRect (left, top, right - left, bottom - top);
}

void renderer_item_group::draw (SkCanvas &canvas, const renderer_state &state, const renderer_config *config) const
{
  if (config)
    {
      if (config->is_interrupted () || m_display == display::NONE)
        return;

      if (config->render_for_selection () && m_is_locked)
        return;
    }


  QRectF transformed_rect = state.transform ().mapRect (bounding_box_impl ());
  QRect result_rect = state.rect ().intersected (bounding_rect (transformed_rect));
  if (result_rect.isNull ())
    return;


  SkBitmap bitmap;
  bitmap.setConfig (SkBitmap::kARGB_8888_Config, result_rect.width (), result_rect.height ());
  bitmap.allocPixels ();
  SkBitmapDevice device(bitmap);
  SkCanvas child_canvas (&device);
  SkPaint paint;

  child_canvas.drawColor (SK_ColorTRANSPARENT, SkXfermode::kSrc_Mode);

  QTransform pixmap_transform = QTransform::fromTranslate (state.rect ().x () - result_rect.x (), state.rect ().y () - result_rect.y ());

  renderer_state new_state (state.rect (), state.transform () * pixmap_transform);

  for (const string &child_name : m_children)
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
      QTransform item_transform = transform () * state.transform ();
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

QRectF renderer_item_group::bounding_box_impl () const
{
  return m_bbox;
}

void renderer_item_group::update_bbox_impl ()
{
  QRectF new_box;
  for (const string &child_name : m_children)
    {
      abstract_renderer_item *renderer_item = container ()->item (child_name);
      if (!renderer_item)
        continue;

      renderer_item->update_bbox ();
      new_box = new_box.united (renderer_item->bounding_box ());
    }

  m_bbox = new_box;
}
