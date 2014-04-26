#ifndef SVG_ITEM_OBSERVER_H
#define SVG_ITEM_OBSERVER_H

#include "svg/undo/undoable.h"

#include <string>

class abstract_attribute;

enum class item_change_type
{
  MYSELF,
  PARENT,
  CHILD,
};

class svg_item_observer : public undoable
{
public:
  svg_item_observer ();
  virtual ~svg_item_observer ();

  virtual void child_added (const string &parent, const string &child_name, int insert_pos);
  virtual void child_removed (const string &parent, const string &child_name, int pos);
  virtual void child_moved (const string &parent, const string &child_name, int old_pos, int new_pos);
  virtual void attribute_change_start (const string &parent, const abstract_attribute *computed_attribute);
  virtual void attribute_change_end (const string &parent, const abstract_attribute *computed_attribute);
  virtual void item_removed (const string &item);
  virtual void layout_changed (const string &sender);
  virtual void item_replaced (const string &sender);
  virtual void item_changed (const string &sender, item_change_type type);
};


#endif // SVG_ITEM_OBSERVER_H
