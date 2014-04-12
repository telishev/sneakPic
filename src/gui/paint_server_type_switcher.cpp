#include "paint_server_type_switcher.h"

#include <QToolButton>
#include <QButtonGroup>

#include "gui_widget_view.h"
#include "connection.h"
#include "gui_model.h"
#include "editor/style_controller.h"
#include "renderer/renderer_paint_server.h"
#include "color_selectors/color_button.h"

Q_DECLARE_METATYPE (item_paint_server);


class color_button_widget : public gui_widget
{
  color_button *m_tool_button;
public:
  color_button_widget (color_button *tool_button) : m_tool_button (tool_button)
  { }

  virtual void set_value (QVariant value) { m_tool_button->set_paint_server (value.value<item_paint_server> ().current_server ()); }
  virtual QVariant value () const { return QVariant (); }
};

class QToolButton_toggle_widget : public gui_widget
{
  QToolButton *m_tool_button;
public:
  QToolButton_toggle_widget (QToolButton *tool_button) : m_tool_button ( tool_button)
  { CONNECT (m_tool_button, &QToolButton::toggled, this, &gui_widget::data_changed); }

  virtual void set_value (QVariant value) { m_tool_button->setChecked (value.toBool ());}
  virtual QVariant value () const { return m_tool_button->isChecked (); }
};

typedef std::pair <renderer_paint_server_type, bool> painter_server_model_role;

static inline painter_server_model_role from_int (int role)
{
  return {(renderer_paint_server_type)(role / 2), (role % 2) != 0 };
}

static inline int to_int (painter_server_model_role role)
{
  return (int)role.first * 2 + (role.second == true);
}

class painter_server_model : public gui_model
{
  gui_model *m_base_model;
public:
  painter_server_model (gui_model *base_model)
  {
    m_base_model = base_model;
    CONNECT (m_base_model, &gui_model::data_changed, this, &painter_server_model::data_changed_slot);
  }

  ~painter_server_model ()
  {
  }

  virtual QVariant data (int int_role) const override
  {
    painter_server_model_role role = from_int (int_role);
    if (role.second)
      return current_active () == role.first;
    else
      {
        item_paint_server server = current_server ();
        server.set_current_type (role.first);
        return QVariant::fromValue (server);
      }
  }

  virtual void set_model_data (const std::map<int, QVariant> &data_map) override
  {
    auto changer = m_base_model->do_multi_change ();
    for (auto && data_pair : data_map)
      {
        painter_server_model_role role = from_int (data_pair.first); 
        if (!role.second || !data_pair.second.toBool ())
          continue;

        item_paint_server server = current_server ();
        server.set_current_type (role.first);
        if (is_selected_fill ())
          changer->set_data (style_controller_role_t::FILL_SERVER, QVariant::fromValue (server));
        else
          changer->set_data (style_controller_role_t::STROKE_SERVER, QVariant::fromValue (server));
      }
  }

private slots:
  void data_changed_slot (const std::set<int> &changes)
  {
    using s = style_controller_role_t;
    std::set<int> roles = {s::FILL_SERVER, s::STROKE_SERVER, s::IS_SELECTED_FILL};
    if (std::find_if (changes.begin (), changes.end (), [&] (int val) {return roles.count (val) > 0;}) == changes.end ())
      return;

    emit data_changed (all_roles ());
  }

private:
  std::set<int> all_roles () const
  {
    using p = renderer_paint_server_type;
    std::set<int> result;
    for (auto val : {p::COLOR, p::LINEAR_GRADIENT, p::NONE, p::RADIAL_GRADIENT})
      {
        result.insert (to_int ({val, true }));
        result.insert (to_int ({val, false}));
      }

    return result;
  }

  bool is_selected_fill () const { return m_base_model->data (style_controller_role_t::IS_SELECTED_FILL).toBool (); }

  item_paint_server current_server () const 
  {
    QVariant data = is_selected_fill () ? m_base_model->data (style_controller_role_t::FILL_SERVER) : m_base_model->data (style_controller_role_t::STROKE_SERVER) ;
    return data.value<item_paint_server> ();
  }

  renderer_paint_server_type current_active () const 
  {
    return current_server ().current_type ();
  }

};

paint_server_type_switcher::paint_server_type_switcher (gui_model *model)
{
  m_base_model = model;
  put_in (m_toolbutton_model, m_base_model);
  put_in (m_view, m_toolbutton_model.get ());

  m_color = create_button (renderer_paint_server_type::COLOR, "Color");
  m_linear_gradient = create_button (renderer_paint_server_type::LINEAR_GRADIENT, "Linear Gradient");
  m_radial_gradient = create_button (renderer_paint_server_type::RADIAL_GRADIENT, "Radial Gradient");
  m_none = create_button (renderer_paint_server_type::NONE, "None");
}

paint_server_type_switcher::~paint_server_type_switcher ()
{

}

QWidget * paint_server_type_switcher::get_color () const
{
  return m_color;
}

QWidget * paint_server_type_switcher::get_linear_gradient () const
{
  return m_linear_gradient;
}

QWidget * paint_server_type_switcher::get_radial_gradient () const
{
  return m_radial_gradient;
}

QWidget * paint_server_type_switcher::get_none () const
{
  return m_none;
}

color_button *paint_server_type_switcher::create_button (renderer_paint_server_type role, QString tooltip)
{
  color_button *button = new color_button;
  button->setCheckable (true);
  button->setToolTip (tooltip);
  m_view->add_gui_widget (to_int ({role, false}), new color_button_widget (button));
  m_view->add_gui_widget (to_int ({role, true }), new QToolButton_toggle_widget (button));
  return button;
}
