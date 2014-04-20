#include "common/common_algorithms.h"
#include "editor/items_selection.h"
#include "editor/transform_handles_editor.h"

#include "renderer/svg_painter.h"
#include "renderer/overlay_renderer.h"
#include "renderer/renderer_overlay_path.h"

#include "svg/attributes/svg_attribute_transform.h"

#include "svg/items/abstract_svg_item.h"
#include "svg/items/svg_graphics_item.h"

#include <QRectF>
#include <QTransform>
#include "path/geom_helpers.h"
#include "svg/svg_document.h"
#include "operations/transform_item_operation.h"

vector<abstract_handle *> transform_handles_editor::get_handles () const
{
  vector<abstract_handle *> handles;
  for (auto &&handle : m_handles)
    {
      handles.push_back (handle.get ());
    }
  return handles;
}

void transform_handles_editor::update_handles_impl ()
{
  items_selection &selection = *m_painter->selection ();
  m_handles.clear ();
  if (selection.empty ())
    return;

  m_contour_renderer->set_visible (false);
  QRectF bbox = selection.get_bbox ();
  for (int i = 0; i < 4; i++)
    m_handles.emplace_back (new transform_handle (static_cast <stretch_type> (i), bbox, *this));
}

transform_handles_editor::transform_handles_editor (overlay_renderer *overlay, svg_painter *painter, actions_applier *applier) : handles_editor (overlay, painter, applier)
{
  put_in (m_contour_renderer);
  overlay->add_item (m_contour_renderer.get (), overlay_layer_type::TEMP);
  put_in (m_transform);
}

transform_handles_editor::~transform_handles_editor ()
{
}

void transform_handles_editor::update_transform (const QTransform &transform)
{
  items_selection &selection = *m_painter->selection ();
  QRectF bbox = selection.get_bbox ();
  QPainterPath path;
  *m_transform = transform;
  path.addRect (bbox);
  path = m_transform->map (path);

  m_contour_renderer->set_painter_path (path);
  m_contour_renderer->set_visible (true);
}

void transform_handles_editor::finalize_transform ()
{
  auto &&selection = *m_painter->selection ();
  transform_item_operation op (m_painter->document ());
  for (auto &&item : selection)
    {
      op.apply_transform (*m_transform, item);
    }
  m_contour_renderer->set_visible (false);
  m_painter->document ()->apply_changes ("Transform");
}

void transform_handles_editor::set_drag_started (bool value)
{
  m_drag_started = value;
}


QPointF transform_handle::get_handle_center () const
{
  switch (m_type)
    {
    case LEFT:
      return {m_drag_started ? m_cur_pos.x () : m_bbox.left () - get_handle_size () * 0.5f, m_bbox.center ().y ()};
    case RIGHT:
      return {m_drag_started ? m_cur_pos.x () : m_bbox.right () + get_handle_size () * 0.5f, m_bbox.center ().y ()};
    case TOP:
      return {m_bbox.center ().x (), m_drag_started ? m_cur_pos.y () : m_bbox.top () - get_handle_size () * 0.5f};
    case BOTTOM:
      return {m_bbox.center ().x (), m_drag_started ? m_cur_pos.y () : m_bbox.bottom () + get_handle_size () * 0.5f};
    }
  return m_bbox.center ();
}

transform_handle::transform_handle (stretch_type type, const QRectF &bbox, transform_handles_editor &editor) : m_editor (editor)
{
  m_bbox = bbox;
  m_type = type;
}

transform_handle::~transform_handle ()

{
}

handle_type transform_handle::get_handle_type () const
{
  return handle_type::DOUBLE_HEADED_ARROW;
}

float transform_handle::handle_rotation () const
{
  switch (m_type)
    {
    case LEFT:
    case RIGHT:
      return 90.0f;
    case TOP:
    case BOTTOM:
      return 0.0f;
    }
   return 0.0f;
}

int transform_handle::get_handle_size () const
{
  return 12;
}

bool transform_handle::start_drag (QPointF local_pos, QTransform /*transform*/)
{
  m_drag_started = true;
  m_editor.set_drag_started (true);
  m_origin_pos = local_pos;
  return true;
}

bool transform_handle::drag (QPointF local_pos, QTransform /*transform*/)
{
  m_cur_pos = local_pos;
  QRectF rect = m_bbox;
  switch (m_type)
    {
    case LEFT:
      rect.setLeft (m_cur_pos.x () + get_handle_size () * 0.5f);
      break;
    case RIGHT:
      rect.setRight (m_cur_pos.x () - get_handle_size () * 0.5f);
      break;
    case TOP:
      rect.setTop (m_cur_pos.y () + get_handle_size () * 0.5f);
      break;
    case BOTTOM:
      rect.setBottom (m_cur_pos.y () - get_handle_size () * 0.5f);
      break;
    }
  m_editor.update_transform (geom::rect2rect (m_bbox, rect));
  return true;
}

bool transform_handle::end_drag (QPointF local_pos, QTransform transform)
{
  drag (local_pos, transform);
  m_editor.set_drag_started (false);
  m_editor.finalize_transform ();
  return true;
}

bool transform_handle::is_visible () const
{
  return (m_editor.drag_started () == m_drag_started);
}

void transform_handle::interrupt_drag ()
{
}
