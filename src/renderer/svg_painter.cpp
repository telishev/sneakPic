#include "renderer/svg_painter.h"

#pragma warning(push, 0)
#include <SkCanvas.h>
#include <SkSurface.h>
#include <SkDevice.h>
#pragma warning(pop)

#include <QMessageBox>
#include <QEvent>
#include <QWheelEvent>
#include <memory>

#include "common/common_utils.h"
#include "common/math_defs.h"

#include "editor/items_selection.h"

#include "gui/gl_widget.h"
#include "gui/mouse_shortcuts_handler.h"
#include "gui/shortcuts_config.h"
#include "gui/settings.h"

#include "renderer/abstract_renderer_item.h"
#include "renderer/renderer_state.h"
#include "renderer/render_cache_id.h"
#include "renderer/rendered_items_cache.h"
#include "renderer/svg_renderer.h"
#include "renderer/qt2skia.h"
#include "renderer/events_queue.h"
#include "renderer/event_transform_changed.h"
#include "renderer/overlay_renderer.h"

#include "svg/svg_document.h"
#include "svg/items/abstract_svg_item.h"
#include "svg/items/svg_items_container.h"



svg_painter::svg_painter (gl_widget *glwidget, const mouse_filter *mouse_filter_object, rendered_items_cache *cache,\
                          events_queue *queue, settings_t *settings)
  : abstract_painter (glwidget, mouse_filter_object)
{
  m_document = nullptr;
  m_cache = cache;
  m_queue = queue;
  m_overlay = new overlay_renderer (m_cache);
  m_selection = new items_selection (m_document);
  m_settings = settings;
  m_mouse_handler = create_mouse_shortcuts ();
}

svg_painter::~svg_painter ()
{
  FREE (m_overlay);
  FREE (m_selection);
  FREE (m_mouse_handler);
}

void svg_painter::reset_transform ()
{
  double doc_width, doc_height;
  m_document->get_doc_dimensions (doc_width, doc_height);
  double scale = qMin (glwidget ()->width () / doc_width, glwidget ()->height () / doc_height);
  m_cur_transform = QTransform::fromScale (scale, scale);
  send_changes (true);
}

void svg_painter::set_document (svg_document *document)
{
  m_document = document;
  if (!m_document)
    return;

  m_overlay->set_document (document);
  set_configure_needed (CONFIGURE_TYPE__ITEMS_CHANGED, 1);
  reset_transform ();
}

unsigned int svg_painter::mouse_event (const mouse_event_t &m_event)
{
  return m_mouse_handler->process_mouse_event (m_event);
}

void svg_painter::draw ()
{
  QPainter painter;
  painter.begin (glwidget ());
  painter.fillRect (glwidget ()->rect(), Qt::white);

  if (!m_document)
    return;

  draw_page (painter);
  //draw_base (painter);
  draw_overlay (painter);

  painter.end ();
}

void svg_painter::configure ()
{
  if (!m_document)
    return;

  if (get_configure_needed (CONFIGURE_TYPE__ITEMS_CHANGED))
    {
      set_configure_needed (CONFIGURE_TYPE__ITEMS_CHANGED, 0);
      set_configure_needed (CONFIGURE_TYPE__REDRAW, 1);
      send_changes (true);
    }

  if (get_configure_needed (CONFIGURE_TYPE__REDRAW))
    {
      update_drawing (m_cur_transform);
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
      send_changes (true);

      glwidget ()->update ();
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
      set_configure_needed (CONFIGURE_TYPE__REDRAW, 1);
      glwidget ()->update ();
      qevent->accept ();
      return true;
    }
  return false;
}

void svg_painter::resizeGL (int width, int height)
{
  FIX_UNUSED (width, height);
}

void svg_painter::update_drawing (QTransform transform)
{
  DO_ON_EXIT ([&] () {m_cache->unlock ();});
  m_cache->lock ();
  double cache_zoom_x = m_cache->zoom_x ();
  double cache_zoom_y = m_cache->zoom_y ();
  double cur_zoom_x = transform.m11 ();
  double cur_zoom_y = transform.m22 ();
  QRectF rect_to_draw = QRectF (glwidget ()->rect ());

  SkBitmap bitmap;
  bitmap.setConfig (SkBitmap::kARGB_8888_Config, rect_to_draw.width (), rect_to_draw.height ());
  bitmap.allocPixels ();
  SkDevice device (bitmap);
  SkCanvas canvas (&device);

  if (!are_equal (cache_zoom_x, cur_zoom_x) || !are_equal (cache_zoom_y, cur_zoom_y))
    {
      QTransform scale_transform = QTransform::fromScale (cache_zoom_x / cur_zoom_x, cache_zoom_y / cur_zoom_y);
      transform = transform * scale_transform;
      canvas.setMatrix (qt2skia::matrix (scale_transform.inverted ()));
      rect_to_draw = scale_transform.mapRect (rect_to_draw);
    }

  canvas.drawColor (SK_ColorTRANSPARENT, SkXfermode::kSrc_Mode);

  render_cache_id id_first, id_last;
  get_cache_id (transform, id_first, id_last, rect_to_draw);

  for (int x = id_first.x (); x <= id_last.x (); x++)
    for (int y = id_first.y (); y <= id_last.y (); y++)
      {
        render_cache_id cur_id (x, y, render_cache_id::ROOT_ITEM);
        SkBitmap bitmap = m_cache->bitmap (cur_id);
        if (bitmap.empty ())
          continue;

        QRectF pixel_rect = cur_id.pixel_rect (transform);
        canvas.drawBitmap (bitmap, SkFloatToScalar (pixel_rect.x ()), SkFloatToScalar (pixel_rect.y ()));
      }

  m_cache->set_current_screen (bitmap);
}

void svg_painter::send_changes (bool interrrupt_rendering)
{
  render_cache_id id_first, id_last;
  get_cache_id (m_cur_transform, id_first, id_last, glwidget ()->rect ());
  int queue_id = m_queue->add_event (new event_transform_changed (id_first, id_last, m_cur_transform, interrrupt_rendering));
  m_queue->wait_for_id (queue_id, 50);
  set_configure_needed (CONFIGURE_TYPE__REDRAW, 1);
}

void svg_painter::get_cache_id (const QTransform &transform, render_cache_id &first, render_cache_id &last, const QRectF &rect) const
{
  first = render_cache_id::get_id_by_pixel_pos (rect.x (), rect.y (), transform);
  last = render_cache_id::get_id_by_pixel_pos (rect.x () + rect.width (),
      rect.y () + rect.height (), transform);
}

abstract_svg_item *svg_painter::get_current_item (const QPoint &pos)
{
  //// first, change current position and zoom to previous zoom level
  double cache_zoom_x = m_cache->zoom_x ();
  double cache_zoom_y = m_cache->zoom_y ();
  double cur_zoom_x = m_cur_transform.m11 ();
  double cur_zoom_y = m_cur_transform.m22 ();
  QTransform scale_transform = QTransform::fromScale (cache_zoom_x / cur_zoom_x, cache_zoom_y / cur_zoom_y);
  QTransform transform = m_cur_transform * scale_transform;
  QPointF scaled_pos = scale_transform.map (QPointF (pos));
  m_cache->lock ();
  DO_ON_EXIT ([&] () {m_cache->unlock ();});

  //// find corresponding block in cache
  render_cache_id id = render_cache_id::get_id_by_pixel_pos (scaled_pos.x (), scaled_pos.y (), transform);
  id.set_id (render_cache_id::ROOT_ITEM_SELECTION);
  SkBitmap bitmap = m_cache->bitmap (id);
  if (bitmap.empty ())
    return nullptr;

  /// pick item id, which is packed in color
  QImage img = qt2skia::qimage (bitmap);
  QRectF block_rect = id.pixel_rect (transform);
  QPointF point_to_pick = scaled_pos - block_rect.topLeft ();
  int block_size = rendered_items_cache::block_pixel_size ();
  DEBUG_ASSERT (   point_to_pick.x () >= 0 && point_to_pick.y () >= 0
                && point_to_pick.x () < block_size && point_to_pick.y () < block_size);
  QColor color (img.pixel (point_to_pick.toPoint ()));
  int item_id = rendered_items_cache::get_id_by_color (color);

  std::string selected_item_name = m_cache->get_selection_name (item_id);
  if (selected_item_name.empty ())
    return nullptr;

  return m_document->item_container ()->get_item (QString::fromStdString (selected_item_name));
}

void svg_painter::draw_base (QPainter &painter)
{
  m_cache->lock ();
  QImage img = qt2skia::qimage (m_cache->get_current_screen ());
  painter.drawImage (img.rect (), img);
  m_cache->unlock ();
}

void svg_painter::draw_overlay (QPainter &painter)
{
  m_overlay->draw (painter, glwidget ()->rect (), m_cur_transform);
}

void svg_painter::draw_page (QPainter &painter)
{
  m_overlay->draw_page (painter, glwidget ()->rect (), m_cur_transform);
}

void svg_painter::select_item (const QPoint &pos)
{
  m_selection->clear ();
  abstract_svg_item *item = get_current_item (pos);
  if (item)
    m_selection->add_item (item);

  m_overlay->selection_changed (m_selection);
  glwidget ()->update ();
}

#define ADD_SHORTCUT(ITEM,FUNC)\
  handler->add_shortcut (cfg->shortcut_mouse (mouse_shortcut_enum::ITEM), MOUSE_FUNC (FUNC));

mouse_shortcuts_handler *svg_painter::create_mouse_shortcuts ()
{
  mouse_shortcuts_handler *handler = new mouse_shortcuts_handler;
  shortcuts_config *cfg = m_settings->shortcuts_cfg ();
  ADD_SHORTCUT (SELECT_ITEM        , select_item (m_event.pos ()));
  ADD_SHORTCUT (START_PAN          , start_pan (m_event.pos ()));
  ADD_SHORTCUT (PAN_PICTURE        , pan_picture (m_event.pos ()));
  ADD_SHORTCUT (FIND_CURRENT_OBJECT, find_current_object (m_event.pos ()));

  return handler;
}

void svg_painter::start_pan (const QPoint &pos)
{
  m_last_transform = m_cur_transform;
  m_drag_start_pos = pos;
}

void svg_painter::pan_picture (const QPoint &pos)
{
  QTransform last_inverted = m_last_transform.inverted ();
  QPointF last_pos_local = last_inverted.map (QPointF (m_drag_start_pos));
  QPointF cur_pos_local = last_inverted.map (QPointF (pos));
  QPointF diff = cur_pos_local - last_pos_local;

  m_cur_transform = QTransform (m_last_transform).translate (diff.x (), diff.y ());
  send_changes (false);
  glwidget ()->update ();
}

void svg_painter::find_current_object (const QPoint &pos)
{
  abstract_svg_item *current_item = get_current_item (pos);
  std::string item_string = current_item ? current_item->name ().toStdString () : std::string ();
  if (item_string != m_overlay->current_item ())
    {
      m_overlay->set_current_item (item_string);
      glwidget ()->update ();
    }
}
