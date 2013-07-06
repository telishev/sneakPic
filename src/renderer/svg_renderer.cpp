#include "svg_renderer.h"

#include <QPainter>
#include <QRectF>
#include <QTransform>

#include "renderer/abstract_renderer_item.h"
#include "renderer/renderer_state.h"
#include "renderer/rendered_items_cache.h"
#include "renderer/render_cache_id.h"

#include "svg/items/abstract_svg_item.h"
#include "common/common_utils.h"



svg_renderer::svg_renderer (rendered_items_cache *cache)
{
  m_cache = cache;
}

svg_renderer::~svg_renderer ()
{

}

void svg_renderer::draw_item (const abstract_svg_item *item, QPainter &painter, const QRectF &rect_to_draw, const QTransform &transform)
{
  const abstract_renderer_item *renderer_item = item->get_renderer_item ();
  if (!renderer_item)
    return;

  renderer_state state;
  state.set_rect (rect_to_draw.toRect ());
  state.set_transform (transform);
  renderer_item->draw (painter, state);
}

void svg_renderer::update_cache_item (const abstract_svg_item *item, const render_cache_id &cache_id, const QTransform &transform)
{
  FIX_UNUSED (item);
  QPixmap pixmap (rendered_items_cache::block_pixel_size (), rendered_items_cache::block_pixel_size ());
  pixmap.fill (Qt::transparent);
  QPainter painter (&pixmap);
  QRectF local_rect = cache_id.pixel_rect (transform);
  QTransform last_inverted = transform.inverted ();
  QPointF last_pos_local = last_inverted.map (QPointF (0, 0));
  QPointF cur_pos_local = last_inverted.map (QPointF (local_rect.x (), local_rect.y ()));
  QPointF diff = -cur_pos_local + last_pos_local;

  QTransform pixmap_transform = QTransform (transform).translate (diff.x (), diff.y ());
  draw_item (item, painter, pixmap.rect (), pixmap_transform);

  painter.end ();
  m_cache->add_pixmap (cache_id, pixmap);
}




