#ifndef SVG_ITEMS_CONTAINER_H
#define SVG_ITEMS_CONTAINER_H

class abstract_svg_item;

#include <map>
#include <QString>

class svg_items_container
 {
  typedef std::map<QString, abstract_svg_item *> map_type;

  map_type m_map;
  int max_id;
public:
  svg_items_container ();
  ~svg_items_container ();

  void add_item (abstract_svg_item *item);
  void remove_item (abstract_svg_item *item);

  abstract_svg_item *get_item (const QString &id) const;
  bool contains (const QString &id) const;

  /// creates unique name in a form "item_id"number
  QString create_unique_name (const char *item_id);

  const map_type &get_items () const { return m_map; }

private:
  /// tries to get some number from a string and update max_id with it to prevent id collisions
  void extract_number (const char *data);
};


#endif // SVG_ITEMS_CONTAINER_H
