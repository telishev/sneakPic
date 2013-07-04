#include "renderer/svg_renderer.h"

#include "common/common_utils.h"

#include "gui/gl_widget.h"

#include "renderer/abstract_renderer_item.h"

#include "svg/svg_document.h"
#include "svg/items/abstract_svg_item.h"

svg_renderer::svg_renderer (gl_widget *glwidget, const mouse_filter *mouse_filter_object)
  : abstract_painter (glwidget, mouse_filter_object)
{
  m_document = nullptr;
  drag_started = false;
}

svg_renderer::~svg_renderer ()
{
}

void svg_renderer::reset_transform ()
{
  double doc_width, doc_height;
  m_document->get_doc_dimensions (doc_width, doc_height);
  double scale = qMin (glwidget ()->width () / doc_width, glwidget ()->height () / doc_height);
  m_cur_transform = QTransform::fromScale (scale, scale);
}

void svg_renderer::set_document (svg_document *document)
{
  m_document = document;
  if (!m_document)
    return;

  set_configure_needed (CONFIGURE_TYPE__ITEMS_CHANGED, 1);
  reset_transform ();
}

unsigned int svg_renderer::mouse_moved (const unsigned char *dragging_buttons, const QPoint &pos, const Qt::KeyboardModifiers &modifiers)
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

unsigned int svg_renderer::mouse_clicked (mouse_filter::mouse_button button, const QPoint &pos, const Qt::KeyboardModifiers &modifiers)
{
  FIX_UNUSED (button, pos, modifiers);
  return 0;
}

unsigned int svg_renderer::mouse_double_clicked (mouse_filter::mouse_button button, const QPoint &pos, const Qt::KeyboardModifiers &modifiers)
{
  FIX_UNUSED (button, pos, modifiers);
  return 0;
}

unsigned int svg_renderer::mouse_pressed (mouse_filter::mouse_button button, const QPoint &pos, const Qt::KeyboardModifiers &modifiers)
{
  FIX_UNUSED (button, pos, modifiers);
  m_last_transform = m_cur_transform;
  m_drag_start_pos = pos;
  drag_started = true;
  return 0;
}

unsigned int svg_renderer::mouse_released (mouse_filter::mouse_button button, const QPoint &pos, const Qt::KeyboardModifiers &modifiers)
{
  FIX_UNUSED (button, pos, modifiers);
  drag_started = false;
  return 0;
}

void svg_renderer::draw ()
{
  QPainter painter;
  painter.begin (glwidget ());
  painter.fillRect (glwidget ()->rect(), Qt::white);

  if (!m_document)
    return;

  painter.setRenderHint(QPainter::Antialiasing);
  painter.setRenderHint(QPainter::HighQualityAntialiasing);
  painter.setTransform (m_cur_transform);
  draw_items (m_document->root (), painter, glwidget ()->rect (), m_cur_transform);
  painter.end ();
}

void svg_renderer::configure ()
{
  if (!m_document)
    return;

  if (get_configure_needed (CONFIGURE_TYPE__ITEMS_CHANGED))
    {
      set_configure_needed (CONFIGURE_TYPE__ITEMS_CHANGED, 0);
      m_document->update_items ();
    }


  return;
}

void svg_renderer::wheelEvent (QWheelEvent *qevent)
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

      glwidget ()->repaint ();
      qevent->accept ();
    }
}

void svg_renderer::leaveEvent (QEvent *qevent)
{
  FIX_UNUSED (qevent);
}

bool svg_renderer::keyReleaseEvent (QKeyEvent * qevent)
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

void svg_renderer::resizeGL (int width, int height)
{
  FIX_UNUSED (width, height);
}

void svg_renderer::draw_items (const abstract_svg_item *item, QPainter &painter, const QRectF &rect_to_draw, QTransform transform)
{
  const abstract_renderer_item *renderer_item = item->get_renderer_item ();
  if (renderer_item)
    draw_single_item (renderer_item, painter, rect_to_draw, transform);

  if (!item->render_children ())
    return;

  for (const abstract_svg_item *child = item->first_child (); child; child = child->next_sibling ())
    draw_items (child, painter, rect_to_draw, transform);
}

void svg_renderer::draw_single_item (const abstract_renderer_item *item, QPainter &painter, const QRectF &rect_to_draw, QTransform transform)
{
  QTransform item_transform = item->transform () * transform;
  QRectF transformed_rect = item_transform.mapRect (item->bounding_box ());
  if (!rect_to_draw.intersects (transformed_rect))
    return;

  painter.setTransform (item_transform);
  item->draw (painter);
}

