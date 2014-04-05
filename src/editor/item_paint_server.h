#ifndef ITEM_PAINT_SERVER_H
#define ITEM_PAINT_SERVER_H

#include <QColor>

class abstract_svg_item;
class items_selection;
class svg_items_container;
class svg_base_attribute_number;
class svg_paint_server;
class svg_base_items_gradient;
class renderer_painter_server_color;
class renderer_linear_gradient;
class renderer_radial_gradient;
class renderer_painter_server_none;
class renderer_base_gradient_item;

class renderer_paint_server;

enum class renderer_paint_server_type;


class item_paint_server
{
  unique_ptr<renderer_painter_server_color> m_color;
  unique_ptr<renderer_linear_gradient> m_linear_gradient;
  unique_ptr<renderer_radial_gradient> m_radial_gradient;
  unique_ptr<renderer_painter_server_none> m_none;
  renderer_paint_server_type m_current_type;
public:
  item_paint_server ();
  ~item_paint_server ();

  item_paint_server (const item_paint_server &rhs);
  item_paint_server (item_paint_server &&rhs);

  item_paint_server& operator = (const item_paint_server &rhs);
  item_paint_server& operator = (item_paint_server &&rhs);

  const renderer_paint_server *current_server () const;
  void set_current_server (const renderer_paint_server *server);
  renderer_paint_server_type current_type () const { return m_current_type; }
  void set_current_type (renderer_paint_server_type type) { m_current_type = type; }

  const renderer_paint_server *server_for_type (renderer_paint_server_type type) const;

  renderer_base_gradient_item *get_gradient_for_change ();

  void set_color (QColor color);
  QColor color () const;

  void set_linear_gradient (const  renderer_linear_gradient &linear_grad);
  const renderer_linear_gradient *linear_gradient () const;

  void set_radial_gradient (const  renderer_radial_gradient &linear_grad);
  const renderer_radial_gradient *radial_gradient () const;

  void create_from_item (abstract_svg_item *item, bool is_fill);
  void apply_to_item (abstract_svg_item *item, bool is_fill) const;

  void create_from_selection (items_selection *selection, bool is_fill);
  void apply_to_selection (items_selection *selection, bool is_fill) const;
};

#endif // ITEM_PAINT_SERVER_H

