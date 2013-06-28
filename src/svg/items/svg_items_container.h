#ifndef SVG_ITEMS_CONTAINER_H
#define SVG_ITEMS_CONTAINER_H

class abstract_svg_item;

#include <map>
#include <QString>

class svg_items_container
{
  typedef std::map<QString, abstract_svg_item *> map_type;

  map_type m_map;
public:
  svg_items_container ();
  ~svg_items_container ();

  void add_item (abstract_svg_item *item);
  void remove_item (abstract_svg_item *item);

  abstract_svg_item *get_item (const QString &id) const;
  bool contains (const QString &id) const;
};


#endif // SVG_ITEMS_CONTAINER_H
