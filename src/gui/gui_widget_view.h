#ifndef GUI_WIDGET_VIEW_H
#define GUI_WIDGET_VIEW_H

#include <QObject>
#include <QPointer>

#include "gui/gui_widget.h"
#include "gui/gui_model.h"
#include "connection.h"

class connection;

class QDoubleSpinBox;


template<typename ROLE>
class gui_widget_view : public gui_model_observer<ROLE> 
{
  struct widget_description
  {
    unique_ptr<gui_widget> widget;
    unique_ptr<connection> widget_connection;

    widget_description (unique_ptr<gui_widget> &&_widget, unique_ptr<connection> &&_connection)
      : widget (std::move (_widget)), widget_connection (std::move (_connection)) {}
    ~widget_description () {}
    widget_description (widget_description &&rhs)
    {
      widget = std::move (rhs.widget);
      widget_connection = std::move (rhs.widget_connection);
    }

  private:
    widget_description (const widget_description &) = delete;
  };

  QPointer <gui_model<ROLE> > m_model;
  std::multimap<ROLE, widget_description> m_widgets;
public:
  gui_widget_view (gui_model<ROLE> *model)
  {
    m_model = model;
    m_model->add_observer (this);
  }
  ~gui_widget_view ()
  {
     if (m_model)
       m_model->remove_observer (this);
  }

  void add_gui_widget (ROLE role, gui_widget *widget)
  {
    auto widget_connect = CONNECT (widget, &gui_widget::data_changed, [&] () { widget_data_changed (role, widget); });
    m_widgets.insert ( std::make_pair (role, widget_description (unique_ptr<gui_widget> (widget), std::move (widget_connect))));
  }

  template<typename T>
  void add_widget (ROLE role, T *widget) { add_gui_widget (role, create_gui_widget (widget)); }

private:
  void widget_data_changed (ROLE role, gui_widget * widget)
  {
    m_model->set_data (role, widget->value ());
  }

  virtual void data_changed_signal (const std::set<ROLE> &changes) override
  {
    for (auto &&role : changes)
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

};

#endif // GUI_WIDGET_VIEW_H
