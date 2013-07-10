#ifndef RENDERER_ITEMS_CONTAINER_H
#define RENDERER_ITEMS_CONTAINER_H

#include <unordered_map>
#include <string>

class abstract_renderer_item;

class renderer_items_container
{
  std::unordered_map<std::string, abstract_renderer_item *> m_items;
  std::string m_root;
public:
  renderer_items_container ();
  ~renderer_items_container ();

  abstract_renderer_item *item (const std::string &name);
  const abstract_renderer_item *item (const std::string &name) const;

  void set_root (const std::string &root);
  const abstract_renderer_item *root () const;
  abstract_renderer_item *root ();

  void add_item (abstract_renderer_item *item);
  void remove_item (const std::string &name);

  void add_child (const std::string &parent, const std::string &child);
};

#endif // RENDERER_ITEMS_CONTAINER_H
