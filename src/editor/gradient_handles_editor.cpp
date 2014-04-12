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
#include "../gui/gui_document.h"

class gradient_handles;

class base_gradient_handle : public base_handle
{
protected:
  QRectF m_bbox;
  renderer_base_gradient_item *m_gradient;
  int m_stop_num;
  QPointF m_drag_start;
  gradient_handles *m_handle;
public:
  base_gradient_handle (gradient_handles *handle, renderer_base_gradient_item *gradient, QRectF bbox, int stop_num)
  {
    m_handle = handle;
    m_stop_num = stop_num;
    m_bbox = bbox;
    m_gradient = gradient;
    set_color (Qt::white);
    set_handle_type (get_handle_type ());
  }

  QColor current_color () const
  {
     return m_gradient->stops ()[m_stop_num].second;
  }

  void set_current_color (QColor color);
  std::pair<std::string, int> get_selection_id () const;
  
  virtual bool start_drag (QPointF local_pos) override
  {
    m_drag_start = local_pos;
    return true;
  }
 
  virtual bool drag (QPointF local_pos) override
  {
    QPointF gradient_pos = gradient_transform ().inverted ().map (local_pos);
    if (is_start ())
      move_start_point (gradient_pos);
    else if (is_end ())
      move_end_point (gradient_pos);
    else
      {
        double new_point = geom::projection_value (start_point_local (), end_point_local (), local_pos);
        m_gradient->stops () [m_stop_num].first = new_point;
      }

    return true;
  }
 
  virtual bool end_drag (QPointF local_pos) override;

  virtual void interrupt_drag () override
  {
    drag (m_drag_start);
  }

  virtual QPointF get_handle_center () const override
  {
    return gradient_transform ().map (cur_point ());
  }

  virtual void draw (SkCanvas &canvas, const renderer_state &state, const renderer_config *config) const override
  {
    /// draw gradient line for first stop
    if (m_stop_num == 0)
      {
        SkPath path;
        path.moveTo (qt2skia::point (start_point_local ()));
        path.lineTo (qt2skia::point (end_point_local ()));
        simple_path_renderer renderer (path, QColor ("slateblue"));
        renderer.draw (canvas, state, config);
      }

    return base_handle::draw (canvas, state, config);
  }

protected:
  virtual QPointF start_point () const = 0;
  virtual QPointF end_point () const = 0;
  virtual void move_start_point (QPointF dst) = 0;
  virtual void move_end_point (QPointF dst) = 0;

protected:
  QPointF start_point_local () const { return  gradient_transform ().map (start_point ()); }
  QPointF end_point_local () const { return  gradient_transform ().map (end_point ()); }

  bool is_start () const  { return m_stop_num == 0; }
  bool is_end () const  { return m_stop_num == (int)m_gradient->stops ().size () - 1; }

  handle_type get_handle_type () const 
  {
    if (is_start ())
      return handle_type::SQUARE;
    else if (is_end ())
      return handle_type::SQUARE;
    else
      return handle_type::DIAMOND;
  }

  QTransform gradient_transform () const
  {
    return  m_gradient->transform () * geom::rect2rect ({0, 0, 1, 1}, m_bbox);
  }

  QPointF cur_point () const
  {
    double coeff = m_gradient->stops ()[m_stop_num].first;
    return start_point () * (1 - coeff) + end_point () * coeff;
  }
};

class linear_gradient_handle : public base_gradient_handle
{
public:
  linear_gradient_handle (gradient_handles *handle, renderer_base_gradient_item *gradient, QRectF bbox, int stop_num)
    : base_gradient_handle (handle, gradient, bbox, stop_num) {}

protected:
  virtual QPointF start_point () const { return { gradient ()->x1 (), gradient ()->y1 () };  }
  virtual QPointF end_point () const { return { gradient ()->x2 (), gradient ()->y2 () };  }
  virtual void move_start_point (QPointF dst) { gradient ()->set_x1 (dst.x ()); gradient ()->set_y1 (dst.y ()); }
  virtual void move_end_point (QPointF dst) { gradient ()->set_x2 (dst.x ()); gradient ()->set_y2 (dst.y ()); }
  renderer_linear_gradient *gradient () const { return static_cast<renderer_linear_gradient *> (m_gradient); }
};

class radial_gradient_handle : public base_gradient_handle
{
  bool m_x_axis;
public:
  radial_gradient_handle (gradient_handles *handle, renderer_base_gradient_item *gradient, QRectF bbox, int stop_num, bool is_x_axis)
    : base_gradient_handle (handle, gradient, bbox, stop_num) {m_x_axis = is_x_axis; }

protected:
  virtual QPointF start_point () const { return { gradient ()->cx (), gradient ()->cy () }; }
  virtual QPointF end_point () const { return get_end_point (m_x_axis); }

  QPointF get_end_point (bool x_axis) const
  {
    if (x_axis)
      return {gradient ()->cx () + gradient ()->r (), gradient ()->cy ()};
    else
      return {gradient ()->cx (), gradient ()->cy () + gradient ()->r ()};
  }

  virtual void move_start_point (QPointF dst) { gradient ()->set_cx (dst.x ()); gradient ()->set_cy (dst.y ()); }
  virtual void move_end_point (QPointF end_dst)
  {
    QPointF src[3] = {{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}};
    QPointF dst[3] = {start_point (), get_end_point (true), get_end_point (false)};
    if (m_x_axis)
      dst[1] = end_dst;
    else
      dst[2] = end_dst;

    for (auto &point : dst)
      point = m_gradient->transform ().map (point);

    QTransform transform = geom::triangle2triangle (src, dst);
    gradient ()->set_transform (transform);
    gradient ()->set_cx (0);
    gradient ()->set_cy (0);
    gradient ()->set_r (1);
  }
  renderer_radial_gradient *gradient () const { return static_cast<renderer_radial_gradient *> (m_gradient); }
};

class gradient_handles : public element_handles
{
  abstract_svg_item *m_item;
  item_paint_server m_server;
  std::vector<abstract_handle *> m_handles;
  bool m_is_fill;
public:
  gradient_handles (abstract_svg_item *item, bool is_fill)
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

  void apply_changes ()
  {
    m_server.apply_to_item (m_item, m_is_fill);
    m_item->document ()->apply_changes ("Gradient Changed");
  }

  ~gradient_handles ()
  {
    for (auto handle : m_handles)
      delete handle;
  }

  abstract_svg_item *item () const { return m_item; }

  virtual vector<abstract_handle *> handles () override
  {
    return m_handles;
  }

};

bool base_gradient_handle::end_drag (QPointF local_pos)
{
  drag (local_pos);
  m_handle->apply_changes ();
  return true;
}

std::pair<std::string, int> base_gradient_handle::get_selection_id () const
{
  auto &handles = m_handle->handles ();
  auto it = std::find (handles.begin (), handles.end (), this);
  DEBUG_ASSERT (it != handles.end ());

  return std::make_pair (m_handle->item ()->name (), (int)(it - handles.begin ()));
}

void base_gradient_handle::set_current_color (QColor color)
{
  m_gradient->stops ()[m_stop_num].second = color;
  m_handle->apply_changes ();
}

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
  m_applier->register_action (gui_action_id::DESELECT_HANDLES, this, &gradient_handles_editor::deselect_handles);
}

gradient_handles_editor::~gradient_handles_editor ()
{

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

