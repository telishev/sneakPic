#ifndef PAINT_SERVER_TYPE_SWITCHER_H
#define PAINT_SERVER_TYPE_SWITCHER_H
#include <QObject>

class gui_widget_view;
class gui_model;
class painter_server_model;
class color_button;
class QWidget;

enum class renderer_paint_server_type;

class paint_server_type_switcher : public QObject
{
  unique_ptr<gui_widget_view> m_view;
  unique_ptr<painter_server_model> m_toolbutton_model;
  gui_model *m_base_model;
  color_button *m_color;
  color_button *m_linear_gradient;
  color_button *m_radial_gradient;
  color_button *m_none;

public:
  paint_server_type_switcher (gui_model *model);
  ~paint_server_type_switcher ();

  QWidget *get_color () const;
  QWidget *get_linear_gradient () const;
  QWidget *get_radial_gradient () const;
  QWidget *get_none () const;

private:
  color_button *create_button (renderer_paint_server_type role, QString tooltip);
};
#endif // PAINT_SERVER_TYPE_SWITCHER_H
