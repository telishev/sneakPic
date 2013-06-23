#ifndef SVG_NAMED_itemS_CONTAINER_H
#define SVG_NAMED_itemS_CONTAINER_H

class svg_named_item;

#include <map>
#include <QString>

class svg_named_items_container
{
  typedef std::map<QString, svg_named_item *> map_type;

  map_type m_map;
public:
  svg_named_items_container ();
  ~svg_named_items_container ();

  void add_item (svg_named_item *item);
  void remove_item (svg_named_item *item);

  svg_named_item *get_item (const QString &name) const;
  bool contains (const QString &name) const;
};


#endif // SVG_NAMED_itemS_CONTAINER_H
