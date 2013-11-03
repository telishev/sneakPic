#include "renderer/overlay_renderer.h"

#include "common/memory_deallocation.h"

#include "renderer/svg_renderer.h"
#include "renderer/qt2skia.h"
#include "renderer/renderer_overlay_root.h"
#include "renderer/rendered_items_cache.h"
#include "renderer/render_cache_id.h"


#pragma warning(push, 0)
#include <SkCanvas.h>
#include <SkSurface.h>
#include <SkDevice.h>
#pragma warning(pop)

#include <QRect>
#include <QTransform>
#include <QImage>
#include <QPainter>
#include <memory>


overlay_renderer::overlay_renderer (rendered_items_cache *cache)
{
  m_overlay_cache = new rendered_items_cache;
  m_renderer = new svg_renderer (m_overlay_cache, nullptr);
  m_base_cache = cache;

  for (int i = 0; i < (int)overlay_layer_type::COUNT; i++)
    m_items[i] = new renderer_overlay_root;
}

overlay_renderer::~overlay_renderer ()
{
  FREE (m_renderer);
  FREE (m_overlay_cache);
  for (int i = 0; i < (int)overlay_layer_type::COUNT; i++)
    FREE (m_items[i]);
}

void overlay_renderer::draw (QPainter &painter, const QRect &rect_to_draw, const QTransform &transform)
{
  SkBitmap bitmap;
  bitmap.setConfig (SkBitmap::kARGB_8888_Config, rect_to_draw.width (), rect_to_draw.height ());
  bitmap.allocPixels ();
  if (m_base_cache)
    bitmap.eraseColor (SK_ColorWHITE);
  else
    bitmap.eraseColor (SK_ColorTRANSPARENT);

  SkDevice device (bitmap);
  SkCanvas canvas (&device);

  render_layer (rect_to_draw, transform, overlay_layer_type::PAGE, bitmap);

  draw_base (canvas);

  /// TODO: cache base layer
  for (int i = (int)overlay_layer_type::BASE; i < (int)overlay_layer_type::COUNT; i++)
    render_layer (rect_to_draw, transform, (overlay_layer_type)i, bitmap);

  painter.drawImage (rect_to_draw, qt2skia::qimage (bitmap));
}

void overlay_renderer::render_layer (const QRect &rect_to_draw, const QTransform &transform, overlay_layer_type layer_type, SkBitmap &bitmap)
{
  renderer_overlay_root *root_item = m_items[(int)layer_type];
  if (root_item->empty ())
    return;

  m_renderer->draw_to_bitmap (rect_to_draw, transform, root_item, &bitmap);
}

void overlay_renderer::add_item (renderable_item *item, overlay_layer_type layer)
{
  m_items[(int)layer]->add_item (item);
}

void overlay_renderer::remove_item (renderable_item *item, overlay_layer_type layer)
{
  m_items[(int)layer]->remove_item (item);
}

void overlay_renderer::draw_base (SkCanvas &canvas)
{
  if (!m_base_cache)
    return;

  m_base_cache->lock ();
  canvas.drawBitmap (m_base_cache->get_current_screen ((int)render_cache_type::ROOT_ITEM), 0, 0);
  m_base_cache->unlock ();
}

