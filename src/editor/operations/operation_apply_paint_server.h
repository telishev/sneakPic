#ifndef OPERATION_APPLY_PAINT_SERVER_H
#define OPERATION_APPLY_PAINT_SERVER_H

class abstract_svg_item;
class renderer_paint_server;
class svg_base_attribute_number;
class svg_items_container;
class svg_paint_server;


class operation_apply_paint_server
{
  const renderer_paint_server *m_server;
  bool m_is_fill;
public:
  operation_apply_paint_server (const renderer_paint_server *server, bool is_fill);
  ~operation_apply_paint_server ();

  void apply (abstract_svg_item *item);
private:
  void set_data (svg_paint_server &server, svg_base_attribute_number &opacity, abstract_svg_item *item) const;
};

#endif // OPERATION_APPLY_PAINT_SERVER_H
