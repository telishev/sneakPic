#ifndef SVG_ITEMS_CONTAINER_H
#define SVG_ITEMS_CONTAINER_H

class abstract_svg_item;

#include <map>
#include <string>

class svg_items_container
{
  typedef map<string, abstract_svg_item *> map_type;

  map_type m_map;
  int max_id;
  string m_root;
public:

  class iterator : public std::iterator<std::forward_iterator_tag, abstract_svg_item *>
  {
    map_type::iterator m_it;
    svg_items_container *m_container;

  private:
    iterator (svg_items_container *container, map_type::iterator it) { m_it = it; m_container = container; };

  public:
    abstract_svg_item *operator* () {return (*m_it).second; };
    bool operator != (const iterator &other) const {return m_it != other.m_it; }
    bool operator == (const iterator &other) const {return m_it == other.m_it; }
    iterator &operator++() { this->m_it++; return *this; };

    friend class svg_items_container;
  };


  svg_items_container ();
  ~svg_items_container ();

  void add_item (abstract_svg_item *item);
  void remove_item (const abstract_svg_item *item);

  abstract_svg_item *get_item (const string &id) const;
  abstract_svg_item *find_item_if (std::function<abstract_svg_item *(bool)> func) const;
  abstract_svg_item *get_editable_item (const string &id) const;
  bool contains (const string &id) const;
  iterator begin () { return iterator (this, m_map.begin ()); }
  iterator end () { return iterator (this, m_map.end ()); }

  /// creates unique name in a form "item_id"number
  string create_unique_name (const char *item_id);

  void set_root (const string &root_id);
  abstract_svg_item *get_root () const;

private:
  /// tries to get some number from a string and update max_id with it to prevent id collisions
  void extract_number (const char *data);
};


#endif // SVG_ITEMS_CONTAINER_H
