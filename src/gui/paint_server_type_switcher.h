#ifndef PAINT_SERVER_TYPE_SWITCHER_H
#define PAINT_SERVER_TYPE_SWITCHER_H
#include <QObject>

template<typename T>
class gui_widget_view;
template<typename T>
class gui_model;
class painter_server_model;
class color_button;
class QWidget;

enum class style_controller_role_t;
enum class renderer_paint_server_type;

typedef std::pair <renderer_paint_server_type, bool> painter_server_model_role;

class paint_server_type_switcher : public QObject
{
  unique_ptr<gui_widget_view<painter_server_model_role>> m_view;
  unique_ptr<painter_server_model> m_toolbutton_model;
  gui_model<style_controller_role_t> *m_base_model;
  color_button *m_color;
  color_button *m_linear_gradient;
  color_button *m_radial_gradient;
  color_button *m_none;

public:
  paint_server_type_switcher (gui_model<style_controller_role_t> *model);
  ~paint_server_type_switcher ();

  QWidget *get_color () const;
  QWidget *get_linear_gradient () const;
  QWidget *get_radial_gradient () const;
  QWidget *get_none () const;

private:
  color_button *create_button (renderer_paint_server_type role, QString tooltip);
};
#endif // PAINT_SERVER_TYPE_SWITCHER_H
