#ifndef GUI_WIDGET_VIEW_H
#define GUI_WIDGET_VIEW_H

#include <QObject>
#include <QPointer>

#include "gui/gui_widget.h"
#include "gui/gui_model.h"
#include "connection.h"

class connection;

class QDoubleSpinBox;


class gui_widget_view : public QObject
{
  Q_OBJECT

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

  gui_model *m_model;
  std::multimap<int, widget_description> m_widgets;
public:
  gui_widget_view (gui_model *model)
  {
    m_model = model;
    CONNECT (m_model, &gui_model::data_changed, this, &gui_widget_view::data_changed);
  }
  ~gui_widget_view ()  {}

  void add_gui_widget (int role, gui_widget *widget)
  {
    auto widget_connect = CONNECT (widget, &gui_widget::data_changed, [&] () { widget_data_changed (role, widget); });
    m_widgets.insert ( std::make_pair (role, widget_description (unique_ptr<gui_widget> (widget), std::move (widget_connect))));
  }

  template<typename T>
  void add_widget (int role, T *widget) { add_gui_widget (role, create_gui_widget (widget)); }

private:
  void widget_data_changed (int role, gui_widget * widget)
  {
    m_model->set_data (role, widget->value ());
  }

private slots: 
  void data_changed (const std::set<int> &changes)
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
