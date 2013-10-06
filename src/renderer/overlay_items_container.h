#ifndef OVERLAY_ITEMS_CONTAINER_H
#define OVERLAY_ITEMS_CONTAINER_H

#include <string>
#include <map>
#include <vector>

class overlay_renderer;
class renderable_item;
class svg_items_container;

enum class overlay_layer_type;
enum class overlay_item_type;

class overlay_items_container
{
  overlay_layer_type m_layer_type;
  overlay_renderer *m_overlay;
  svg_items_container *m_container;
  std::map<std::string, renderable_item *> m_obj_map; ///< map from object name to corresponding overlay item
public:
  overlay_items_container (overlay_renderer *overlay, svg_items_container *container, overlay_layer_type layer_type);
  ~overlay_items_container ();

  void update_items ();

protected:
  svg_items_container *svg_container () const;
  void add_svg_item (const std::string &object);
  void svg_item_changed (const std::string &object);
  void remove_svg_item (const std::string &object);
  void clear_items ();

  renderable_item *create_overlay_for_item (const std::string &object, overlay_item_type overlay_type) const;

  virtual renderable_item *create_overlay_item (const std::string &object) const = 0;
};


#endif // OVERLAY_ITEMS_CONTAINER_H
