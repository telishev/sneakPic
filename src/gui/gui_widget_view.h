#ifndef GUI_WIDGET_VIEW_H
#define GUI_WIDGET_VIEW_H

#include <QObject>

#include "gui/create_gui_widget.h"

class gui_model;
enum class gui_model_role_t;
class connection;

class QDoubleSpinBox;

class gui_widget : public QObject
{
  Q_OBJECT

public:
  virtual ~gui_widget () {}

  virtual void set_value (QVariant value) = 0;
  virtual QVariant value () const = 0;

signals:
  void data_changed ();
};

class gui_widget_view : public QObject
{
  Q_OBJECT

  struct widget_description
  {
    unique_ptr<gui_widget> widget;
    unique_ptr<connection> widget_connection;

    widget_description (unique_ptr<gui_widget> &&_widget, unique_ptr<connection> &&_connection);
    ~widget_description ();
    widget_description (widget_description &&rhs);

  private:
    widget_description (const widget_description &);
  };

  gui_model *m_model;
  std::multimap<gui_model_role_t, widget_description> m_widgets;
public:
  gui_widget_view (gui_model *model);
  ~gui_widget_view ();

  void add_gui_widget (gui_model_role_t role, gui_widget *widget);

  template<typename T>
  void add_widget (gui_model_role_t role, T *widget) { add_gui_widget (role, create_gui_widget (widget)); }

private slots:
  void model_data_changed (const std::set<gui_model_role_t> &changes);

private:
  void widget_data_changed (gui_model_role_t role, gui_widget * widget);
};

#endif // GUI_WIDGET_VIEW_H
