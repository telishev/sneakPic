#include "renderer/svg_painter.h"

#include "skia/skia_includes.h"

#include <memory>

#include <QEvent>
#include <QLocale>
#include <QMessageBox>
#include <QRectF>
#include <QStatusBar>
#include <QWheelEvent>

#include "common/common_utils.h"
#include "common/debug_utils.h"
#include "common/math_defs.h"
#include "common/memory_deallocation.h"

#include "editor/items_selection.h"
#include "editor/tools/abstract_tool.h"

#include "gui/actions_applier.h"
#include "gui/connection.h"
#include "gui/canvas_widget_t.h"
#include "gui/gui_action_id.h"
#include "gui/mouse_shortcuts_handler.h"
#include "gui/settings.h"
#include "gui/shortcuts_config.h"

#include "renderer/abstract_renderer_item.h"
#include "renderer/current_item_outline_renderer.h"
#include "renderer/event_transform_changed.h"
#include "renderer/events_queue.h"
#include "renderer/items_selection_renderer.h"
#include "renderer/overlay_renderer.h"
#include "renderer/qt2skia.h"
#include "renderer/render_cache_id.h"
#include "renderer/rendered_items_cache.h"
#include "renderer/renderer_items_container.h"
#include "renderer/renderer_overlay_path.h"
#include "renderer/renderer_page.h"
#include "renderer/renderer_state.h"
#include "renderer/rubberband_selection.h"
#include "renderer/svg_renderer.h"

#include "svg/items/abstract_svg_item.h"
#include "svg/items/items_comparison.h"
#include "svg/items/svg_items_container.h"
#include "svg/svg_document.h"
#include "svg/svg_utils.h"
#include "svg/items/svg_graphics_item.h"
#include "gui/gui_document.h"
#include "editor/tools/tools_container.h"
#include "svg/undo/undo_handler.h"

using namespace std::placeholders;

svg_painter::svg_painter (canvas_widget_t *canvas_widget, rendered_items_cache *cache, events_queue *queue, svg_document *document,
                          settings_t *settings, gui_document *gui_doc)
  : abstract_painter (canvas_widget)
{
  m_renderer_page = nullptr;
  m_current_tool = nullptr;
  m_document = nullptr;
  m_gui_document = gui_doc;
  m_cache = cache;
  m_queue = queue;
  m_overlay = new overlay_renderer (m_cache);
  m_settings = settings;
  m_selection = new items_selection (document->item_container ());
  m_actions_applier = new actions_applier;
  m_mouse_handler = new mouse_shortcuts_handler (m_settings->shortcuts_cfg (),
                                                 std::bind (&svg_painter::process_mouse_event, this, _1, _2));
  update_status_bar_widgets ();
  set_document (document);

  CONNECT (m_selection, &items_selection::selection_changed, this, &svg_painter::selection_changed);

  create_mouse_shortcuts ();
  m_actions_applier->register_action (gui_action_id::DELETE_ITEMS, this, &svg_painter::remove_items_in_selection);
  m_actions_applier->add_drag_shortcut (mouse_drag_shortcut_t::COLOR_PICKER_DRAG, this, &svg_painter::pick_color_start, &svg_painter::pick_color_drag, &svg_painter::pick_color_end);
  m_actions_applier->add_shortcut (mouse_shortcut_t::COLOR_PICKER_CLICK, this, &svg_painter::pick_color_click);
  m_actions_applier->register_action (gui_action_id::RAISE_OBJECT, this, &svg_painter::raise_object);
  m_actions_applier->register_action (gui_action_id::LOWER_OBJECT, this, &svg_painter::lower_object);
  m_color_picker_area_preview.reset (new renderer_overlay_path ());
  m_color_picker_area_preview->set_color (Qt::white);
  m_color_picker_area_preview->set_xfer_mode (SkXfermode::Mode::kDifference_Mode);
  m_overlay->add_item (m_color_picker_area_preview.get (), overlay_layer_type::TEMP);
}

svg_painter::~svg_painter ()
{
  FREE (m_mouse_handler);
  FREE (m_overlay);
  FREE (m_selection);

  FREE (m_selection_renderer);
  FREE (m_item_outline);
  FREE (m_renderer_page);
  FREE (m_actions_applier);
}

bool svg_painter::pick_color_start (const QPoint &pos)
{
  m_color_picker_pos = pos;
  m_color_picker_area_preview->set_visible (true);
  pick_color_drag (pos);
  return true;
}

bool svg_painter::pick_color_click (const QPoint &pos)
{
  pick_color_start (pos);
  pick_color_end (pos);
  return true;
}

bool svg_painter::pick_color_drag (const QPoint &pos)
{
  QPointF new_pos (get_local_pos (pos));
  QPainterPath path;
  QPointF local_color_picker_pos = get_local_pos (m_color_picker_pos);
  float distance = QLineF (local_color_picker_pos, new_pos).length ();
  path.addEllipse (local_color_picker_pos, distance, distance);
  m_color_picker_area_preview->set_painter_path (path);
  update ();
  return true;
}

bool svg_painter::pick_color_end (const QPoint &pos)
{
  QPointF final_pos = pos;

  m_color_picker_area_preview->set_visible (false);
  QPainterPath path;
  float distance = qMax (QLineF (m_color_picker_pos, final_pos).length (), 1.0);
  path.addEllipse (m_color_picker_pos, distance, distance);
  m_color_picker_area_preview->set_painter_path (path);

  svg_renderer renderer (nullptr, nullptr);
  unique_ptr<renderer_items_container> container (m_document->create_rendered_items (nullptr));
  QRectF rect (path.boundingRect ());
  QTransform transform;
  QPointF leftTopRectPnt = get_local_pos (rect.topLeft ());
  QPointF LeftTopScreenPnt = get_local_pos (QPointF (0, 0));
  transform.translate (LeftTopScreenPnt.x () - leftTopRectPnt.x (),LeftTopScreenPnt.y () -leftTopRectPnt.y ());
  transform *= m_cur_transform;
  path.translate (-rect.left (), -rect.top ());
  rect = path.boundingRect ();
  unique_ptr<SkBitmap> bitmap (renderer.draw_to_bitmap (rect.toRect (), transform, container->root (), Qt::white));
  QImage image = qt2skia::qimage (*bitmap.get ());
  QImage imageCut (image.width (), image.height (), QImage::Format_ARGB32);
  {
    imageCut.fill (0);
    QPainter painter (&imageCut);
    painter.setClipPath (path);
    painter.drawImage (0, 0, image);
  }

  int r = 0, g = 0, b = 0, c = 0;
  for (int j = 0; j < imageCut.height (); j++)
    {
      QRgb *rgb = reinterpret_cast<QRgb *> (imageCut.scanLine (j));
      for (int i = 0; i < imageCut.width (); i++)
        {
          if (qAlpha (rgb[i]) > 0)
            {
              r += qRed (rgb[i]);
              g += qGreen (rgb[i]);
              b += qBlue (rgb[i]);
              c++;
            }
        }
    }
  if (c == 0)
    return true;

  emit color_picked (QColor (r / c, g / c, b / c));
  update ();
  return true;
}

void svg_painter::update_status_bar_widgets ()
{
  emit zoom_description_changed (QString ("%1 %2 ").arg ( QLocale ().toString (m_cur_transform.m11 () * 100, 'f', 2)).arg (QLocale ().percent ()));
}

void svg_painter::reset_transform ()
{
  double doc_width, doc_height;
  svg_utils::get_doc_dimensions (m_document, doc_width, doc_height);
  double scale = qMin (canvas_widget ()->width () / doc_width, canvas_widget ()->height () / doc_height);
  m_cur_transform = QTransform::fromScale (scale, scale);
  send_changes (true);
  update_status_bar_widgets ();
}

void svg_painter::set_document (svg_document *document)
{
  m_document = document;

  double width, height;
  svg_utils::get_doc_dimensions (document, width, height);
  create_overlay_containers ();
  m_renderer_page = new renderer_page (width, height);
  m_overlay->add_item (m_renderer_page, overlay_layer_type::PAGE);

  reset_transform ();
  CONNECT (m_document, &svg_document::items_changed, this, &svg_painter::items_changed);
}

unsigned int svg_painter::mouse_event (const mouse_event_t &m_event)
{
  return m_mouse_handler->process_mouse_event (m_event);
}

void svg_painter::draw ()
{
  QPainter painter;
  painter.begin (canvas_widget ());
  painter.fillRect (canvas_widget ()->rect(), Qt::white);

  QRect rect = canvas_widget ()->rect ();
  m_overlay->draw (painter, rect, m_cur_transform);

  if (m_current_tool)
    m_current_tool->draw (painter, rect, m_cur_transform);
  painter.end ();
}

void svg_painter::configure ()
{
  if (get_configure_needed (configure_type::ITEMS_CHANGED))
    {
      m_selection->remove_unavailable_items ();
      m_item_outline->update_items ();
      set_configure_needed (configure_type::REDRAW_BASE);
      set_configure_needed (configure_type::SELECTION_CHANGED);
    }

  if (get_configure_needed (configure_type::SELECTION_CHANGED))
    {
      emit selection_changed_signal ();
      m_selection_renderer->update_items ();
    }

  if (m_current_tool)
    m_current_tool->configure ();

  /// must be last
  if (get_configure_needed (configure_type::REDRAW_BASE))
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
      update_status_bar_widgets ();

      canvas_widget ()->update ();
      qevent->accept ();
    }
}

void svg_painter::leaveEvent (QEvent *qevent)
{
  FIX_UNUSED (qevent);
}

void svg_painter::resizeEvent (QResizeEvent * /*qevent*/)
{
  send_changes (false);
  canvas_widget ()->update ();
}

void svg_painter::update_drawing (QTransform transform)
{
  svg_renderer renderer (m_cache, nullptr);
  renderer.update_drawing (transform, QRectF (canvas_widget ()->rect ()), (int)render_cache_type::ROOT_ITEM);
}

void svg_painter::send_changes (bool interrrupt_rendering)
{

  auto object_pair = render_cache_id::get_id_for_pixel_rect (m_cur_transform, canvas_widget ()->rect (), (int)render_cache_type::ROOT_ITEM);
  m_queue->add_event_and_wait (new event_transform_changed (object_pair.first, object_pair.second, m_cur_transform, interrrupt_rendering));
  set_configure_needed (configure_type::REDRAW_BASE);
}

abstract_svg_item *svg_painter::get_current_item (const QPoint &pos)
{
  int radius = 3;
  set<abstract_svg_item *> items;
  /// find all items near current mouse pos
  for (int offset_x = -radius; offset_x <= radius; offset_x++)
    for (int offset_y = -radius; offset_y <= radius; offset_y++)
      {
        if (offset_x * offset_x + offset_y * offset_y > radius * radius)
          continue;

        abstract_svg_item *item = get_current_item_for_point (pos + QPoint (offset_x, offset_y));
        if (item)
          items.insert (item);
      }

  if (items.empty ())
    return nullptr;

  return *std::max_element (items.begin (), items.end (), items_comparison_z_order ());
}

bool svg_painter::do_select_item (const QPoint &pos, bool clear_selection)
{
  abstract_svg_item *item = get_current_item (pos);

  if (item != nullptr && m_selection->contains (item->name ()) && clear_selection && m_gui_document->get_tools_container ()->current_tool_id () == gui_action_id::TOOL_SELECTOR)
    {
      emit switch_transform_handles_request ();
      return true;
    }

  if (clear_selection)
    m_selection->clear ();

  if (item != nullptr)
    {
      if (m_selection->contains (item->name ()) && !clear_selection)
        m_selection->remove_item (item);
      else
        m_selection->add_item (item);
    }

  canvas_widget ()->update ();
  return true;
}

bool svg_painter::select_item (const QPoint &pos)
{
  return do_select_item (pos, true);
}

void svg_painter::create_mouse_shortcuts ()
{
  m_actions_applier->add_shortcut (mouse_shortcut_t::SELECT_ITEM, this, &svg_painter::select_item);
  m_actions_applier->add_shortcut (mouse_shortcut_t::ADD_ITEM_TO_SELECTION, this, &svg_painter::add_item_to_selection);
  m_actions_applier->add_shortcut (mouse_shortcut_t::FIND_CURRENT_OBJECT, this, &svg_painter::find_current_object);

  m_actions_applier->add_drag_shortcut (mouse_drag_shortcut_t::PAN, this,
    &svg_painter::start_pan, &svg_painter::pan_picture, &svg_painter::end_pan);
}

bool svg_painter::start_pan (const QPoint &pos)
{
  m_last_transform = m_cur_transform;
  m_drag_start_pos = pos;
  return true;
}

bool svg_painter::pan_picture (const QPoint &pos)
{
  QTransform last_inverted = m_last_transform.inverted ();
  QPointF last_pos_local = last_inverted.map (QPointF (m_drag_start_pos));
  QPointF cur_pos_local = last_inverted.map (QPointF (pos));
  QPointF diff = cur_pos_local - last_pos_local;

  m_cur_transform = QTransform (m_last_transform).translate (diff.x (), diff.y ());
  send_changes (false);
  canvas_widget ()->update ();
  return true;
}

bool svg_painter::find_current_object (const QPoint &pos)
{
  abstract_svg_item *current_item = get_current_item (pos);
  string item_string = current_item ? current_item->name () : string ();
  if (item_string != m_item_outline->current_item ())
    {
      m_item_outline->set_current_item (item_string);
      canvas_widget ()->update ();
    }

  return false;
}


void svg_painter::create_overlay_containers ()
{
  m_selection_renderer = new items_selection_renderer (item_container (), m_selection);
  m_item_outline = new current_item_outline_renderer (item_container ());
  m_overlay->add_item (m_item_outline, overlay_layer_type::BASE);
  m_overlay->add_item (m_selection_renderer, overlay_layer_type::TEMP);
}

void svg_painter::items_changed ()
{
  set_configure_needed (configure_type::ITEMS_CHANGED);
  configure ();
  canvas_widget ()->update ();
}

void svg_painter::set_current_tool (abstract_tool *tool)
{
  if (m_current_tool)
    m_current_tool->deactivate ();

  m_current_tool = tool;
  if (m_current_tool)
    m_current_tool->activate ();

  items_changed ();
  canvas_widget ()->update ();
}

QPointF svg_painter::get_local_pos (const QPointF &mouse_pos) const
{
  return m_cur_transform.inverted ().map (mouse_pos);
}

void svg_painter::redraw ()
{
  canvas_widget ()->update ();
}

svg_items_container *svg_painter::item_container () const
{
  return m_document->item_container ();
}

void svg_painter::selection_changed ()
{
  set_configure_needed (configure_type::SELECTION_CHANGED);
}

abstract_svg_item *svg_painter::get_current_item_for_point (const QPoint &pos)
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
  DO_ON_EXIT (m_cache->unlock ());

  //// find corresponding block in cache
  render_cache_id id = render_cache_id::get_id_by_pixel_pos (scaled_pos.x (), scaled_pos.y (), transform, (int)render_cache_type::ROOT_ITEM_SELECTION);
  SkBitmap bitmap = m_cache->bitmap (id);
  if (bitmap.empty ())
    return nullptr;

  /// pick item id, which is packed in color
  QImage img = qt2skia::qimage (bitmap);
  QRectF block_rect = id.pixel_rect (transform);
  QPoint point_to_pick = (scaled_pos - block_rect.topLeft ()).toPoint ();
  int block_size = rendered_items_cache::block_pixel_size ();
  if (point_to_pick.x () >= img.width ())
    point_to_pick.setX (img.width () - 1);

  if (point_to_pick.y () >= img.height ())
    point_to_pick.setY (img.height () - 1);

  DEBUG_ASSERT (   point_to_pick.x () >= 0 && point_to_pick.y () >= 0
                   && point_to_pick.x () < block_size && point_to_pick.y () < block_size);
  QColor color (img.pixel (point_to_pick));
  int item_id = rendered_items_cache::get_id_by_color (color);

  string selected_item_name = m_cache->get_selection_name (item_id);
  if (selected_item_name.empty ())
    return nullptr;

  return m_document->item_container ()->get_editable_item (selected_item_name);
}

bool svg_painter::action_triggered (gui_action_id id)
{
  if (m_current_tool)
    if (m_current_tool->action_triggered (id))
      return true;

  return m_actions_applier->apply_action (id);
}

enum class z_direction {
  UP,
  DOWN,
};

void svg_painter::move_selected_items_by_z (z_direction direction)
{
  if (m_selection->count () == 0)
    return;

  // copy selection
  vector<svg_graphics_item *> selection_graphic_items;
  for (auto item : *m_selection)
    {
      auto graphics_item = item->to_graphics_item ();
      selection_graphic_items.push_back (graphics_item);
    }

  if (selection_graphic_items.size () == 0)
    return;

  abstract_svg_item *parent_item = selection_graphic_items[0]->parent ();

  if (!parent_item)
    return;

  for (auto && item : selection_graphic_items)
    {
      if (item->parent () != parent_item)
        return; // if parents are different - do nothing
    }

  // otherwise - sort by depth according to direction
  sort (selection_graphic_items.begin (), selection_graphic_items.end (), [&] (abstract_svg_item * itemA, abstract_svg_item * itemB)
  {
    bool less = (itemA->child_index () < itemB->child_index ());
    return (direction == z_direction::DOWN) ? !less : less;
  });
  // for example for down direction we are checking top-most item first:

  int limit_index = 0;

  // for each element finding element that lies between them and previous in the list which is appropriate for moving (intersects bounding box of our item)
  for (auto it = selection_graphic_items.begin (); it != selection_graphic_items.end (); ++it)
    {
      if (it + 1 == selection_graphic_items.end ())
        {
          switch (direction)
            {
            case z_direction::UP:
              limit_index = (int) parent_item->children_count ();
              break;
            case z_direction::DOWN:
              limit_index = -1;
              break;
            }
        }
      else
        limit_index = (* (it + 1))->child_index ();

      auto has_intersection = [&] (int index) {
                                                 svg_graphics_item *graphics_item = parent_item->child (index)->to_graphics_item ();
                                                 return graphics_item ? graphics_item->bbox ().intersects ((*it)->bbox ()) : false;
                                              };
      int found_index = -1;
      switch (direction)
        {
        case z_direction::UP:
          {
            for (int i = (*it)->child_index () + 1; i < limit_index; i++)
            {
              if (has_intersection (i))
              {
                found_index = i;
                break;
              }
            }
          }
          break;
        case z_direction::DOWN:
          {
            for (int i = (*it)->child_index () - 1; i > limit_index; i--)
            {

              if (has_intersection (i))
              {
                found_index = i;
                break;
              }
            }
          }
          break;
        }
      if (found_index != -1)
        {
          switch (direction)
            {
            case z_direction::UP:
              parent_item->move_child (found_index + 1, *it);
              break;
            case z_direction::DOWN:
              parent_item->move_child (found_index, *it);
              break;
            }

        }
    }
}

bool svg_painter::lower_object ()
{
  move_selected_items_by_z (z_direction::DOWN);
  document ()->apply_changes ("Lower");
  return true;
}

bool svg_painter::raise_object ()
{
  move_selected_items_by_z (z_direction::UP);
  document ()->apply_changes ("Raise");
  return true;
}

bool svg_painter::remove_items_in_selection ()
{
  if (m_selection->count () == 0)
    return true;

  std::vector<int> selected_id;

  for (auto item : *m_selection)
    {
      if (!item)
        continue;

      selected_id.push_back (item->undo_id ());
    }

  for (int id : selected_id)
    {
      abstract_svg_item *item = dynamic_cast<abstract_svg_item *> (m_document->get_undo_handler ()->get_item (id));
      if (item && item->parent ())
        item->parent ()->remove_child (item);
    }

  m_selection->clear ();
  document ()->apply_changes ("Remove");
  return true;
}

bool svg_painter::process_mouse_event (const mouse_event_t &event, mouse_shortcut_enum_union action)
{
  if (m_current_tool)
    if (m_current_tool->mouse_event (event, action))
      return true;

  return m_actions_applier->apply_action (event, action);
}

bool svg_painter::add_item_to_selection (const QPoint &pos)
{
  return do_select_item (pos, false);
}

style_controller * svg_painter::get_style_controller () const
{
  return m_gui_document->get_style_controller ();
}

