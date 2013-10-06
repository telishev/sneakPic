#ifndef ITEMS_SELECTION_H
#define ITEMS_SELECTION_H

#include "renderer/overlay_items_container.h"

#include <set>

class abstract_svg_item;
class svg_document;
class QRectF;

class items_selection : public overlay_items_container /// TODO: create separate class for items selection rendering
{
  typedef std::set<std::string> set_type;
  set_type m_selection;
public:
  items_selection (overlay_renderer *overlay, svg_items_container *container);
  ~items_selection ();

  int selected_count () const;
  const set_type &selection () const { return m_selection; }

  bool contains (const std::string &name) const;

  void add_item (const abstract_svg_item *item);
  void add_item (const std::string &item_name);

  void clear ();

  void add_items_for_rect (const QRectF &rect);

protected:
  virtual renderable_item *create_overlay_item (const std::string &object) const override;

private:
  void add_items_for_rect (const QRectF &rect, const abstract_svg_item *root);
};

#endif // ITEMS_SELECTION_H
