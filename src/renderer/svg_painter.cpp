#include "renderer/svg_painter.h"

#include "common/common_utils.h"

#include "gui/gl_widget.h"

#include "renderer/abstract_renderer_item.h"
#include "renderer/renderer_state.h"
#include "renderer/render_cache_id.h"
#include "renderer/rendered_items_cache.h"
#include "renderer/svg_renderer.h"
#include "renderer/qt2skia.h"

#include "svg/svg_document.h"
#include "svg/items/abstract_svg_item.h"

#pragma warning(push, 0)
#include <SkCanvas.h>
#include <SkSurface.h>
#include <SkDevice.h>
#pragma warning(pop)



svg_painter::svg_painter (gl_widget *glwidget, const mouse_filter *mouse_filter_object, rendered_items_cache *cache)
  : abstract_painter (glwidget, mouse_filter_object)
{
  m_document = nullptr;
  drag_started = false;
  m_cache = cache;
  m_renderer = new svg_renderer (cache);
}

svg_painter::~svg_painter ()
{
  FREE (m_renderer);
}

void svg_painter::reset_transform ()
{
  double doc_width, doc_height;
  m_document->get_doc_dimensions (doc_width, doc_height);
  double scale = qMin (glwidget ()->width () / doc_width, glwidget ()->height () / doc_height);
  m_cur_transform = QTransform::fromScale (scale, scale);
}

void svg_painter::set_document (svg_document *document)
{
  m_document = document;
  if (!m_document)
    return;

  set_configure_needed (CONFIGURE_TYPE__ITEMS_CHANGED, 1);
  reset_transform ();
}

unsigned int svg_painter::mouse_moved (const unsigned char *dragging_buttons, const QPoint &pos, const Qt::KeyboardModifiers &modifiers)
{
  FIX_UNUSED (dragging_buttons, pos, modifiers);
  if (drag_started)
    {
      QTransform last_inverted = m_last_transform.inverted ();
      QPointF last_pos_local = last_inverted.map (QPointF (m_drag_start_pos));
      QPointF cur_pos_local = last_inverted.map (QPointF (pos));
      QPointF diff = cur_pos_local - last_pos_local;

      m_cur_transform = QTransform (m_last_transform).translate (diff.x (), diff.y ());
      glwidget ()->repaint ();
    }

  return 0;
}

unsigned int svg_painter::mouse_clicked (mouse_filter::mouse_button button, const QPoint &pos, const Qt::KeyboardModifiers &modifiers)
{
  FIX_UNUSED (button, pos, modifiers);
  return 0;
}

unsigned int svg_painter::mouse_double_clicked (mouse_filter::mouse_button button, const QPoint &pos, const Qt::KeyboardModifiers &modifiers)
{
  FIX_UNUSED (button, pos, modifiers);
  return 0;
}

unsigned int svg_painter::mouse_pressed (mouse_filter::mouse_button button, const QPoint &pos, const Qt::KeyboardModifiers &modifiers)
{
  FIX_UNUSED (button, pos, modifiers);
  m_last_transform = m_cur_transform;
  m_drag_start_pos = pos;
  drag_started = true;
  return 0;
}

unsigned int svg_painter::mouse_released (mouse_filter::mouse_button button, const QPoint &pos, const Qt::KeyboardModifiers &modifiers)
{
  FIX_UNUSED (button, pos, modifiers);
  drag_started = false;
  return 0;
}

void svg_painter::draw ()
{
  QPainter painter;
  painter.begin (glwidget ());
  painter.fillRect (glwidget ()->rect(), Qt::white);

  if (!m_document)
    return;

  painter.setRenderHint(QPainter::Antialiasing);
  painter.setRenderHint(QPainter::HighQualityAntialiasing);
  painter.setRenderHint (QPainter::SmoothPixmapTransform);
  draw_items (m_document->root (), painter, glwidget ()->rect (), m_cur_transform);
  painter.end ();
}

void svg_painter::configure ()
{
  if (!m_document)
    return;

  if (get_configure_needed (CONFIGURE_TYPE__ITEMS_CHANGED))
    {
      set_configure_needed (CONFIGURE_TYPE__ITEMS_CHANGED, 0);
      m_document->update_items ();
      m_cache->zoom_level_changed ();
    }


  return;
}

void svg_painter::wheelEvent (QWheelEvent *qevent)
{
  if (qevent->orientation () == Qt::Vertical)
    {
      double scale = pow (1.2, qevent->delta() / 240.0);
      FIX_UNUSED (scale);
      QPointF event_pos_local_before = m_cur_transform.inverted ().map (QPointF (qevent->pos ()));
      m_cur_transform.scale (scale, scale);
      QPointF event_pos_local_after = m_cur_transform.inverted ().map (QPointF (qevent->pos ()));
      QPointF vector = event_pos_local_after - event_pos_local_before;
      m_cur_transform.translate (vector.x (), vector.y ());
      m_cache->zoom_level_changed ();

      glwidget ()->repaint ();
      qevent->accept ();
    }
}

void svg_painter::leaveEvent (QEvent *qevent)
{
  FIX_UNUSED (qevent);
}

bool svg_painter::keyReleaseEvent (QKeyEvent * qevent)
{
  if (qevent->key () == Qt::Key_1 && qevent->modifiers () == Qt::NoModifier)
  {
    if (!m_document)
      return true;
    reset_transform ();
    glwidget ()->repaint ();
    qevent->accept ();
    return true;
  }
  return false;
}

void svg_painter::resizeGL (int width, int height)
{
  FIX_UNUSED (width, height);
}

void svg_painter::draw_items (const abstract_svg_item *item, QPainter &painter, const QRectF &rect_to_draw, QTransform transform)
{
  const abstract_renderer_item *renderer_item = item->get_renderer_item ();
  if (!renderer_item)
    return;

  SkBitmap bitmap;
  bitmap.setConfig (SkBitmap::kARGB_8888_Config, rect_to_draw.width (), rect_to_draw.height ());
  bitmap.allocPixels ();
  SkDevice device (bitmap);
  SkCanvas canvas (&device);
  canvas.drawColor (SK_ColorTRANSPARENT, SkXfermode::kSrc_Mode);

#if 0
  m_renderer->draw_item (item, canvas, rect_to_draw, transform);
#else

  QRectF mapped_rect = transform.inverted ().mapRect (rect_to_draw);
  render_cache_id id_first = render_cache_id::get_id_by_pos (mapped_rect.x (), mapped_rect.y (), transform);
  render_cache_id id_last = render_cache_id::get_id_by_pos (mapped_rect.x () + mapped_rect.width (),
    mapped_rect.y () + mapped_rect.height (), transform);

  m_renderer->update_cache_items (item, id_first, id_last, transform);
  for (int x = id_first.x (); x <= id_last.x (); x++)
    for (int y = id_first.y (); y <= id_last.y (); y++)
      {
        render_cache_id cur_id (x, y);
        SkBitmap bitmap = m_cache->bitmap (cur_id);
        if (bitmap.empty ())
          continue;

        QRectF pixel_rect = cur_id.pixel_rect (transform);
        /// set rect width and height to integer values to eliminate rounding errors
        int block_size = rendered_items_cache::block_pixel_size ();
        pixel_rect.setWidth (block_size);
        pixel_rect.setHeight (block_size);

        canvas.drawBitmap (bitmap, SkFloatToScalar (pixel_rect.x ()), SkFloatToScalar (pixel_rect.y ()));
      }
#endif

  painter.drawImage (rect_to_draw, qt2skia::qimage (bitmap));
}
