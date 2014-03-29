#ifndef OPERATION_SET_GRADIENT_H
#define OPERATION_SET_GRADIENT_H

class svg_paint_server;
class svg_document;
class renderer_base_gradient_item;
class svg_base_items_gradient;
class svg_items_container;
class abstract_svg_item;

class operation_set_gradient
{
  const renderer_base_gradient_item *m_gradient;
public:
  operation_set_gradient (const renderer_base_gradient_item *gradient);
  ~operation_set_gradient ();

  void apply (svg_document *document, svg_paint_server *paint_server);
private:
  svg_base_items_gradient *get_suitable_gradient (svg_paint_server *server, svg_items_container *container) const;
  void fill_gradient (svg_base_items_gradient *gradient);
  void add_stops_to_gradient (svg_document *document, svg_base_items_gradient *gradient);
  void fill_linear_gradient (svg_base_items_gradient *gradient);
  void fill_radial_gradient (svg_base_items_gradient *gradient);
};

#endif // OPERATION_SET_GRADIENT_H
