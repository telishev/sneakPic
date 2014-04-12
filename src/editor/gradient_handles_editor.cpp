#include "gradient_handles_editor.h"

#include <QTransform>

#include "common/debug_utils.h"

#include "path/geom_helpers.h"
#include "path/svg_path_geom.h"

#include "editor/element_handles.h"
#include "editor/base_handle.h"
#include "editor/item_paint_server.h"
#include "editor/style_controller.h"

#include "renderer/renderer_items_gradient.h"
#include "renderer/svg_painter.h"
#include "renderer/anchor_handle_renderer.h"
#include "renderer/path_preview_renderer.h"
#include "renderer/qt2skia.h"

#include "svg/items/svg_graphics_item.h"
#include "svg/svg_document.h"

#include "gui/shortcuts_config.h"
#include "gui/actions_applier.h"
#include "gui/gui_action_id.h"
#include "gui/multi_gui_model.h"
#include "gui/gui_document.h"
#include "gradient_handles.h"


class gradient_handles_color_model : public gui_model
{
  gradient_handles_editor *m_handles_editor;
  bool m_is_active = false;
public:
  gradient_handles_color_model (gradient_handles_editor *handles_editor)
  {
    m_handles_editor = handles_editor;
  }

  void set_is_active (bool active)
  {
    m_is_active = active;
    emit_data_changed ();
  }

  virtual QVariant data (int role) const override
  {
    base_gradient_handle *current_selection = m_handles_editor->current_selection ();
    switch (role)
    {
      case style_controller_role_t::CURRENT_COLOR_TEMP:
      case style_controller_role_t::CURRENT_COLOR:
        return current_selection ? current_selection->current_color () : QVariant ();
      case multi_gui_model_role::AVAILABLE: return m_is_active && current_selection;
    }

    return {};
  }

  virtual void set_model_data (const std::map<int, QVariant> &data_map) override
  {
    base_gradient_handle *current_selection = m_handles_editor->current_selection ();
    for (auto && data : data_map)
      {
        if (data.first != style_controller_role_t::CURRENT_COLOR || !current_selection)
          continue;

        current_selection->set_current_color (data.second.value<QColor> ());
        emit_data_changed ();
      }
  }

  void emit_data_changed () {emit data_changed ({style_controller_role_t::CURRENT_COLOR}); }
};

gradient_handles_editor::gradient_handles_editor (overlay_renderer *overlay, svg_painter *painter, actions_applier *applier)
: handles_editor (overlay, painter, applier)
{
  m_disable_deselect = false;
  put_in (m_color_model, this);
  m_painter->get_gui_document ()->add_color_model (m_color_model.get ());
  m_applier->add_shortcut (mouse_shortcut_t::SELECT_HANDLE, this, &gradient_handles_editor::select_handle);
  m_applier->add_shortcut (mouse_shortcut_t::ADD_GRADIENT_STOP, this, &gradient_handles_editor::add_gradient_stop);
  m_applier->register_action (gui_action_id::DESELECT_HANDLES, this, &gradient_handles_editor::deselect_handles);
  m_applier->register_action (gui_action_id::DELETE_HANDLES, this, &gradient_handles_editor::remove_handles);
}

gradient_handles_editor::~gradient_handles_editor ()
{
  m_painter->get_gui_document ()->remove_color_model (m_color_model.get ());
}

element_handles * gradient_handles_editor::create_handles_for_item (abstract_svg_item *item)
{
  return new gradient_handles (item, m_painter->get_style_controller ()->is_selected_fill ());
}

void gradient_handles_editor::update_handles_impl ()
{
  auto cur_selected = current_selection ();
  if (cur_selected)
    cur_selected->set_selected (true);
  else
    set_selected_handle ({});
}

bool gradient_handles_editor::select_handle (const QPointF &pos)
{
  auto gradient_handle = dynamic_cast<base_gradient_handle *> (get_handle_by_pos (pos));
  if (!gradient_handle)
    return deselect_handles ();

  auto cur_selected = current_selection ();
  if (cur_selected)
    cur_selected->set_selected (false);

  set_selected_handle (gradient_handle->get_selection_id ());
  gradient_handle->set_selected (true);
  m_disable_deselect = true;
  m_applier->apply_action (gui_action_id::DESELECT_HANDLES);
  m_disable_deselect = false;

  m_painter->update ();
  return true;
}

base_gradient_handle * gradient_handles_editor::current_selection () const
{
  gradient_handles *handles = dynamic_cast<gradient_handles *> (handles_for_item (m_selected_handle.first));
  if (!handles)
    return nullptr;

  auto &handles_vector = handles->handles ();
  if (m_selected_handle.second >= (int)handles_vector.size ())
    return nullptr;

  return dynamic_cast<base_gradient_handle *>(handles_vector[m_selected_handle.second]);
}

bool gradient_handles_editor::deselect_handles ()
{
  if (m_disable_deselect || m_selected_handle.first.empty ())
    return false;

  auto cur_selected = current_selection ();
  if (cur_selected)
    cur_selected->set_selected (false);
  set_selected_handle ({});

  m_painter->update ();
  return true;
}

void gradient_handles_editor::set_page_active (bool is_active)
{
  m_color_model->set_is_active (is_active);
}

void gradient_handles_editor::set_selected_handle (std::pair<std::string, int> handle)
{
  m_selected_handle = handle;
  m_color_model->emit_data_changed ();
}

bool gradient_handles_editor::remove_handles ()
{
  base_gradient_handle *selection = current_selection ();
  if (selection && selection->remove ())
    {
      set_selected_handle ({});
      update_handles ();
      return true;
    }

  return false;
}

bool gradient_handles_editor::add_gradient_stop (const QPointF &pos)
{
  const double max_distance = 10;
  double distance = max_distance;
  gradient_handles *nearest_handles = nullptr;

  for (const auto & handle_pair : *this)
    {
      gradient_handles *handles = dynamic_cast<gradient_handles *> (handle_pair.second.get ());
      if (!handles)
        continue;

      double cur_distance = handles->distance_to_line (pos, m_painter->cur_transform ());
      if (cur_distance < distance)
        {
          distance = cur_distance;
          nearest_handles = handles;
        }
    }

  if (!nearest_handles)
    return false;

  nearest_handles->add_handle (pos, m_painter->cur_transform ());
  set_selected_handle ({});
  update_handles ();
  return true;
}

vector<abstract_handle *> gradient_handles::handles ()
{
  return m_handles;
}

abstract_svg_item * gradient_handles::item () const
{
  return m_item;
}

gradient_handles::~gradient_handles ()
{
  for (auto handle : m_handles)
    delete handle;
}

void gradient_handles::apply_changes ()
{
  m_server.apply_to_item (m_item, m_is_fill);
  m_item->document ()->apply_changes ("Gradient Changed");
}

gradient_handles::gradient_handles (abstract_svg_item *item, bool is_fill)
{
  m_is_fill = is_fill;
  m_item = item;
  m_server.create_from_item (m_item, m_is_fill);
  renderer_base_gradient_item *gradient = m_server.get_gradient_for_change ();
  QRectF rect = item->to_graphics_item ()->bbox ();
  if (m_server.current_type () == renderer_paint_server_type::LINEAR_GRADIENT)
    {
      for (size_t i = 0; i < gradient->stops ().size (); i++)
        {
          m_handles.push_back (new linear_gradient_handle (this, gradient, rect, (int)i));
        }
    }
  else if (m_server.current_type () == renderer_paint_server_type::RADIAL_GRADIENT)
    {
      for (size_t i = 0; i < gradient->stops ().size (); i++)
        for (bool x_axis : {true, false})
          m_handles.push_back (new radial_gradient_handle (this, gradient, rect, (int)i, x_axis));
    }
}

double gradient_handles::distance_to_line (QPointF pos, QTransform cur_transform) const
{
  if (m_handles.empty ())
    return 1000;

  if (m_server.current_type () == renderer_paint_server_type::LINEAR_GRADIENT)
    return static_cast<linear_gradient_handle *> (m_handles.back ())->distance_to_line (pos, cur_transform);
  else
    {
      auto handle2 = static_cast<radial_gradient_handle *> (m_handles.back ());
      auto handle1 = static_cast<radial_gradient_handle *> (m_handles[m_handles.size () - 2]);
      return std::min (handle1->distance_to_line (pos, cur_transform), handle2->distance_to_line (pos, cur_transform));
    }
}

void gradient_handles::add_handle (QPointF pos, QTransform cur_transform)
{
  QPointF local_pos = cur_transform.inverted ().map (pos);
  if (m_server.current_type () == renderer_paint_server_type::LINEAR_GRADIENT)
    return static_cast<linear_gradient_handle *> (m_handles.back ())->add_handle (local_pos);
  else
    {
      auto handle2 = static_cast<radial_gradient_handle *> (m_handles.back ());
      auto handle1 = static_cast<radial_gradient_handle *> (m_handles[m_handles.size () - 2]);
      if (handle1->distance_to_line (pos, cur_transform) <  handle2->distance_to_line (pos, cur_transform))
        handle1->add_handle (local_pos);
      else
        handle2->add_handle (local_pos);
    }
}
