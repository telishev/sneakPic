#ifndef RENDERER_ITEMS_CONTAINER_H
#define RENDERER_ITEMS_CONTAINER_H

#include <unordered_map>
#include <string>

class abstract_renderer_item;
class rendered_items_cache;

class renderer_items_container
{
  std::unordered_map<string, abstract_renderer_item *> m_items;
  string m_root;
  int m_last_id;
  rendered_items_cache *m_cache;
public:
  renderer_items_container ();
  ~renderer_items_container ();

  abstract_renderer_item *item (const string &name);
  const abstract_renderer_item *item (const string &name) const;

  void set_root (const string &root);
  const abstract_renderer_item *root () const;
  abstract_renderer_item *root ();

  void add_item (abstract_renderer_item *item);
  void remove_item (const string &name);

  void add_child (const string &parent, const string &child);

  void change_item (abstract_renderer_item *item);

  void set_cache (rendered_items_cache *cache) { m_cache = cache; }
};

#endif // RENDERER_ITEMS_CONTAINER_H
