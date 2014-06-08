#include "common/common_algorithms.h"
#include "editor/items_selection.h"
#include "editor/transform_handles_editor.h"

#include "renderer/svg_painter.h"
#include "renderer/overlay_renderer.h"
#include "renderer/renderer_overlay_path.h"

#include "svg/attributes/svg_attribute_transform.h"

#include "svg/items/abstract_svg_item.h"
#include "svg/items/svg_base_graphics_item.h"

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
  switch (m_cur_handles_type)
    {
    case handles_type::STRETCH:
      for (int i = 0; i < (int) transform_type::SKEW_ROTATE_FIRST; i++)
        m_handles.emplace_back (new transform_handle (static_cast <transform_type> (i), bbox, *this));
      break;
    case handles_type::SKEW_ROTATE:
      for (int i = (int)  SKEW_ROTATE_FIRST; i < (int) transform_type::COUNT; i++)
        m_handles.emplace_back (new transform_handle (static_cast <transform_type> (i), bbox, *this));
      break;
    case handles_type::COUNT:
      break;
    }
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
  QPainterPath path;
  for (auto &&selection_item : selection)
    {
      auto &&gr_item = selection_item->to_base_graphics_item ();
      if (gr_item != nullptr)
        path.addPath (gr_item->get_transformed_boundaries ());
    }

  path.addRect (selection.get_bbox ());

  *m_transform = transform;
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

void transform_handles_editor::interrupt ()
{
  m_contour_renderer->set_visible (false);
  m_drag_started = false;
}

void transform_handles_editor::set_drag_started (bool value)
{
  m_drag_started = value;
}

void transform_handles_editor::switch_handles_type ()
{
  m_cur_handles_type = (handles_type) (((int) m_cur_handles_type + 1) % (int) handles_type::COUNT);
  update_handles ();
  m_painter->update ();
}

QPointF transform_handle::get_handle_center () const
{
  double x_shift = get_handle_size () * 0.5 / m_editor.painter ().cur_transform ().m11 ();
  double y_shift = get_handle_size () * 0.5 / m_editor.painter ().cur_transform ().m22 ();
  switch (m_type)
    {
    case STRETCH_LEFT:
    case SKEW_LEFT:
      return {m_bbox.left () - x_shift, m_bbox.center ().y ()};
    case STRETCH_RIGHT:
    case SKEW_RIGHT:
      return {m_bbox.right () + x_shift, m_bbox.center ().y ()};
    case SKEW_TOP:
    case STRETCH_TOP:
      return {m_bbox.center ().x (), m_bbox.top () - y_shift};
    case STRETCH_BOTTOM:
    case SKEW_BOTTOM:
      return {m_bbox.center ().x (), m_bbox.bottom () + y_shift};
    case STRETCH_TOPLEFT:
    case ROTATE_TOPLEFT:
      return {m_bbox.left () - x_shift, m_bbox.top () - y_shift};
    case STRETCH_TOPRIGHT:
    case ROTATE_TOPRIGHT:
      return { m_bbox.right () + x_shift, m_bbox.top () - y_shift};
    case STRETCH_BOTTOMLEFT:
    case ROTATE_BOTTOMLEFT:
      return {m_bbox.left () - x_shift, m_bbox.bottom () + y_shift};
    case STRETCH_BOTTOMRIGHT:
    case ROTATE_BOTTOMRIGHT:
      return {m_bbox.right () + x_shift, m_bbox.bottom () + y_shift};
    case COUNT:
      return {};
    }
  return m_bbox.center ();
}

transform_handle::transform_handle (transform_type type, const QRectF &bbox, transform_handles_editor &editor) : m_editor (editor)
{
  m_bbox = bbox;
  m_type = type;
}

transform_handle::~transform_handle ()

{
}

handle_type transform_handle::get_handle_type () const
{
switch (m_type)
  {
    case STRETCH_LEFT:
    case STRETCH_RIGHT:
    case STRETCH_TOP:
    case STRETCH_BOTTOM:
    case STRETCH_TOPLEFT:
    case STRETCH_TOPRIGHT:
    case STRETCH_BOTTOMLEFT:
    case STRETCH_BOTTOMRIGHT:
    case SKEW_LEFT:
    case SKEW_RIGHT:
    case SKEW_BOTTOM:
    case SKEW_TOP:
      return handle_type::DOUBLE_HEADED_ARROW;
    case ROTATE_TOPLEFT:
    case ROTATE_TOPRIGHT:
    case ROTATE_BOTTOMLEFT:
    case ROTATE_BOTTOMRIGHT:
      return handle_type::ROTATE_ARROW;
    case COUNT:
      break;
  }
  return handle_type::CIRCLE;
}

float transform_handle::handle_rotation () const
{
  switch (m_type)
    {
    case STRETCH_LEFT:
    case STRETCH_RIGHT:
    case SKEW_BOTTOM:
    case SKEW_TOP:
      return 90.0f;
    case STRETCH_TOP:
    case STRETCH_BOTTOM:
    case SKEW_LEFT:
    case SKEW_RIGHT:
      return 0.0f;
    case STRETCH_TOPLEFT:
    case STRETCH_BOTTOMRIGHT:
      return 135.0f;
    case STRETCH_BOTTOMLEFT:
    case STRETCH_TOPRIGHT:
      return 45.0f;
    case ROTATE_TOPLEFT:
      return 90.0f;
    case ROTATE_TOPRIGHT:
      return 180.0f;
    case ROTATE_BOTTOMRIGHT:
      return 270.0f;
    case ROTATE_BOTTOMLEFT:
      return 0.0f;
    case COUNT:
      return {};
    }
   return 0.0f;
}

int transform_handle::get_handle_size () const
{
  return 14;
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
  if (m_type < SKEW_ROTATE_FIRST)
    {
      switch (m_type)
        {
        case STRETCH_LEFT:
          rect.setLeft (m_cur_pos.x () + get_handle_size () * 0.5f);
          break;
        case STRETCH_RIGHT:
          rect.setRight (m_cur_pos.x () - get_handle_size () * 0.5f);
          break;
        case STRETCH_TOP:
          rect.setTop (m_cur_pos.y () + get_handle_size () * 0.5f);
          break;
        case STRETCH_BOTTOM:
          rect.setBottom (m_cur_pos.y () - get_handle_size () * 0.5f);
          break;
        case STRETCH_BOTTOMLEFT:
          rect.setBottom (m_cur_pos.y () - get_handle_size () * 0.5f);
          rect.setLeft (m_cur_pos.x () + get_handle_size () * 0.5f);
          break;
        case STRETCH_BOTTOMRIGHT:
          rect.setBottom (m_cur_pos.y () - get_handle_size () * 0.5f);
          rect.setRight (m_cur_pos.x () - get_handle_size () * 0.5f);
          break;
        case STRETCH_TOPLEFT:
          rect.setTop (m_cur_pos.y () + get_handle_size () * 0.5f);
          rect.setLeft (m_cur_pos.x () + get_handle_size () * 0.5f);
          break;
        case STRETCH_TOPRIGHT:
          rect.setTop (m_cur_pos.y () + get_handle_size () * 0.5f);
          rect.setRight (m_cur_pos.x () - get_handle_size () * 0.5f);
          break;
        default:
          return true;
        }
      m_editor.update_transform (geom::rect2rect (m_bbox, rect));
    }
  else
    {
      QTransform m_transform;
      m_transform.translate (m_bbox.center ().x (), m_bbox.center ().y ());
      switch (m_type)
        {
        case SKEW_LEFT:
          m_transform.shear (0.0, (m_bbox.center ().y () - m_cur_pos.y ()) / (0.5 * m_bbox.width ()));
          break;
        case SKEW_RIGHT:
          m_transform.shear (0.0, (m_cur_pos.y () - m_bbox.center ().y ()) / (0.5 * m_bbox.width ()));
          break;
        case SKEW_BOTTOM:
          m_transform.shear ((m_cur_pos.x () - m_bbox.center ().x ()) / (0.5 * m_bbox.height ()), 0.0);
          break;
        case SKEW_TOP:
          m_transform.shear ((m_bbox.center ().x () - m_cur_pos.x ()) / (0.5 * m_bbox.height ()), 0.0);
          break;
        case ROTATE_TOPLEFT:
          m_transform.rotate (geom::rad2deg (geom::angle_between (m_cur_pos - m_bbox.center (), m_bbox.topLeft () - m_bbox.center ())));
          break;
        case ROTATE_TOPRIGHT:
          m_transform.rotate (geom::rad2deg (geom::angle_between (m_cur_pos - m_bbox.center (), m_bbox.topRight () - m_bbox.center ())));
          break;
        case ROTATE_BOTTOMLEFT:
          m_transform.rotate (geom::rad2deg (geom::angle_between (m_cur_pos - m_bbox.center (), m_bbox.bottomLeft () - m_bbox.center ())));
          break;
        case ROTATE_BOTTOMRIGHT:
          m_transform.rotate (geom::rad2deg (geom::angle_between (m_cur_pos - m_bbox.center (), m_bbox.bottomRight () - m_bbox.center ())));
          break;
        default:
          return true;
        }
      m_transform.translate (-m_bbox.center ().x (), -m_bbox.center ().y ());
      m_editor.update_transform (m_transform);
    }
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
  return !m_editor.drag_started ();
}

void transform_handle::interrupt_drag ()
{
  m_editor.interrupt ();
}
