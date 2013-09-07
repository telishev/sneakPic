#ifndef OVERLAY_ITEMS_CONTAINER_H
#define OVERLAY_ITEMS_CONTAINER_H

#include <string>
#include <map>
#include <vector>

class overlay_renderer;
class abstract_renderer_item;
class svg_items_container;

enum class overlay_layer_type;
enum class overlay_item_type;

class overlay_items_container
{
  overlay_layer_type m_layer_type;
  overlay_renderer *m_overlay;
  std::multimap<std::string, std::string> m_obj_map; ///< map from object name to corresponding overlay items
public:
  overlay_items_container (overlay_renderer *overlay, overlay_layer_type layer_type);
  ~overlay_items_container ();

  void update_items ();

protected:
  svg_items_container *svg_container () const;
  void add_svg_item (const std::string &object);
  void svg_item_changed (const std::string &object);
  void remove_svg_item (const std::string &object);
  void clear_items ();

  std::vector<abstract_renderer_item *> create_overlay_for_item (const std::string &object, overlay_item_type overlay_type) const;
  std::vector<abstract_renderer_item *> single_item_vector (abstract_renderer_item *item) const;

  virtual std::vector<abstract_renderer_item *> create_overlay_item (const std::string &object) const = 0;
};


#endif // OVERLAY_ITEMS_CONTAINER_H
