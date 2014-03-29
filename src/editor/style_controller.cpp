#include "editor/style_controller.h"

#include "editor/items_selection.h"

#include "renderer/svg_painter.h"

#include "svg/svg_document.h"
#include "gui/connection.h"

Q_DECLARE_METATYPE (Qt::PenJoinStyle);
Q_DECLARE_METATYPE (Qt::PenCapStyle);
Q_DECLARE_METATYPE (item_paint_server);

style_controller::style_controller (settings_t * /*settings_arg*/)
{
  m_current_style = selected_style::EDITOR_STYLE;
  m_painter = nullptr;
  m_is_selected_fill = true;
}

const item_paint_style *style_controller::active_style () const
{
  return &m_styles[(int) m_current_style];
}

void style_controller::set_painter (svg_painter *painter)
{
  CONNECT (painter, &svg_painter::selection_changed_signal, this, &style_controller::selection_or_items_changed);
  CONNECT (painter, &svg_painter::color_picked, this, &style_controller::update_from_color_picker);
  m_painter = painter;
}

void style_controller::selection_or_items_changed ()
{
  item_paint_style &style = m_styles[(int) m_current_style];
  style.fill ().create_from_selection (m_painter->selection (), true);
  style.stroke ().create_from_selection (m_painter->selection (), false);
  style.stroke_cfg ().create_from_selection (m_painter->selection ());
  send_items_changed ();
}

void style_controller::update_from_color_picker (const QColor &color)
{
  active_server ()->set_color (color);
  active_server ()->apply_to_selection (m_painter->selection (), true);
  m_painter->document ()->apply_changes ("Pick Color");
  send_items_changed ();
}

const item_paint_server * style_controller::active_server () const
{
  return m_is_selected_fill ? &active_style ()->fill () : &active_style ()->stroke ();
}

item_paint_server * style_controller::active_server ()
{
  return m_is_selected_fill ? &m_styles[(int) m_current_style].fill () : &m_styles[(int) m_current_style].stroke ();
}

QVariant style_controller::data (style_controller_role_t role) const
{
  const item_paint_style *style = active_style ();
  const stroke_config &stroke_cfg = style->stroke_cfg ();
  const item_paint_server *cur_server = active_server ();

  switch (role)
    {
    case style_controller_role_t::STROKE_WIDTH: return stroke_cfg.width ();
    case style_controller_role_t::STROKE_MITER: return stroke_cfg.miterlimit ();
    case style_controller_role_t::LINECAP: return stroke_cfg.linecap ();
    case style_controller_role_t::LINEJOIN: return stroke_cfg.linejoin ();
    case style_controller_role_t::CURRENT_COLOR: return cur_server->color ();
    case style_controller_role_t::CURRENT_COLOR_TEMP: return cur_server->color ();
    case style_controller_role_t::IS_SELECTED_FILL: return m_is_selected_fill;
    case style_controller_role_t::FILL_SERVER: return QVariant::fromValue (style->fill ());
    case style_controller_role_t::STROKE_SERVER: return QVariant::fromValue (style->stroke ());
    }

  return QVariant ();
}

void style_controller::set_model_data (const std::map<style_controller_role_t, QVariant> &data_map)
{
  if (data_map.empty () || !m_painter)
    return;

  item_paint_style &style = m_styles[(int) m_current_style];
  item_paint_server *cur_server = active_server ();
  stroke_config &stroke_cfg = style.stroke_cfg ();
  bool need_apply = std::find_if (data_map.begin (), data_map.end (),
                                  [] (const pair<style_controller_role_t, QVariant> &val)
                                       { return    val.first != style_controller_role_t::CURRENT_COLOR_TEMP
                                                && val.first != style_controller_role_t::IS_SELECTED_FILL; }) != data_map.end ();

  bool stroke_changed = false;
  bool color_changed = false;
  bool fill_color_changed = false;
  bool stroke_color_changed = false;
  std::set<style_controller_role_t> update_set = get_change_set (data_map);
  std::set<style_controller_role_t> all_items = all_items_set ();

  for (auto &&data_pair : data_map)
    {
      QVariant value = data_pair.second;
      switch (data_pair.first)
        {
        case style_controller_role_t::STROKE_WIDTH: stroke_cfg.set_width (value.toDouble ()); stroke_changed = true; break;
        case style_controller_role_t::STROKE_MITER: stroke_cfg.set_miterlimit (value.toDouble ()); stroke_changed = true; break;
        case style_controller_role_t::LINECAP: stroke_cfg.set_linecap (value.value<Qt::PenCapStyle> ()); stroke_changed = true; break;
        case style_controller_role_t::LINEJOIN: stroke_cfg.set_linejoin (value.value<Qt::PenJoinStyle> ()); stroke_changed = true; break;
        case style_controller_role_t::CURRENT_COLOR: cur_server->set_color (value.value<QColor> ()); color_changed = true; break;
        case style_controller_role_t::CURRENT_COLOR_TEMP: cur_server->set_color (value.value<QColor> ()); color_changed = true; break;
        case style_controller_role_t::IS_SELECTED_FILL: m_is_selected_fill = value.toBool (); update_set.insert (all_items.begin (), all_items.end ()); break;
        case style_controller_role_t::FILL_SERVER: style.fill () = value.value<item_paint_server> (); color_changed = true; fill_color_changed = true; break;
        case style_controller_role_t::STROKE_SERVER: style.stroke () = value.value<item_paint_server> (); color_changed = true; stroke_color_changed = true; break;
        }
    }

  items_selection *selection = m_painter->selection ();
  if (stroke_changed)
    stroke_cfg.apply_to_selection (selection);

  if (color_changed)
    {
      cur_server->apply_to_selection (selection, is_selected_fill ());
      if (fill_color_changed)
        style.fill ().apply_to_selection (selection, true);
      if (stroke_color_changed)
        style.stroke ().apply_to_selection (selection, false);

      update_set.insert (style_controller_role_t::CURRENT_COLOR);
      update_set.insert (style_controller_role_t::CURRENT_COLOR_TEMP);
      update_set.insert (style_controller_role_t::FILL_SERVER);
      update_set.insert (style_controller_role_t::STROKE_SERVER);
    }

  if (need_apply)
    m_painter->document ()->apply_changes ("Change Style");

  m_painter->document ()->redraw ();
  emit data_changed (update_set);
}

void style_controller::send_items_changed ()
{
  emit data_changed (all_items_set ());
}

std::set<style_controller_role_t> style_controller::all_items_set () const
{
  using g = style_controller_role_t;
  return {g::STROKE_WIDTH, g::STROKE_MITER, g::LINECAP, g::LINEJOIN, g::CURRENT_COLOR, g::CURRENT_COLOR_TEMP, g::FILL_SERVER, g::STROKE_SERVER};
}

void style_controller::set_current_style (selected_style /*style*/)
{
}

