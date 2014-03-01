#include "gui/gui_widget_view.h"

#include "gui/connection.h"
#include "gui/gui_model.h"




gui_widget_view::gui_widget_view (gui_model *model)
{
  m_model = model;
  CONNECT (m_model, &gui_model::data_changed, this, &gui_widget_view::model_data_changed);
}

gui_widget_view::~gui_widget_view ()
{

}

void gui_widget_view::model_data_changed (const std::set<gui_model_role_t> &changes)
{
  for (auto role : changes)
    {
      auto it = m_widgets.equal_range (role);
      for (auto cur_it = it.first; cur_it != it.second; ++cur_it)
        {
          cur_it->second.widget_connection->disconnect ();
          cur_it->second.widget->set_value (m_model->data (cur_it->first));
          cur_it->second.widget_connection->connect ();
        }
    }
}

void gui_widget_view::add_gui_widget (gui_model_role_t role, gui_widget *widget)
{
  auto widget_connect = CONNECT (widget, &gui_widget::data_changed, [&] () { widget_data_changed (role, widget); });
  m_widgets.insert ( std::make_pair (role, widget_description (unique_ptr<gui_widget> (widget), std::move (widget_connect))));
  
}

void gui_widget_view::widget_data_changed (gui_model_role_t role, gui_widget * widget)
{
  m_model->set_data (role, widget->value ());
}

gui_widget_view::widget_description::widget_description (unique_ptr<gui_widget> &&_widget, unique_ptr<connection> &&_connection) 
  : widget (std::move (_widget)), widget_connection (std::move (_connection))
{

}

gui_widget_view::widget_description::widget_description (widget_description &&rhs)
{
  widget = std::move (rhs.widget);
  widget_connection = std::move (rhs.widget_connection);
}

gui_widget_view::widget_description::~widget_description ()
{

}

