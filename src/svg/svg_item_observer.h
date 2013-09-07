#ifndef SVG_ITEM_OBSERVER_H
#define SVG_ITEM_OBSERVER_H

#include "svg/undo/undoable.h"

#include <string>

class abstract_attribute;

class svg_item_observer : public undoable
{
public:
  svg_item_observer ();
  virtual ~svg_item_observer ();

  virtual void child_added (const std::string &parent, const std::string &child_name, int insert_pos);
  virtual void child_removed (const std::string &parent, const std::string &child_name, int pos);
  virtual void child_moved (const std::string &parent, const std::string &child_name, int old_pos, int new_pos);
  virtual void attribute_change_start (const std::string &parent, const abstract_attribute *computed_attribute);
  virtual void attribute_change_end (const std::string &parent, const abstract_attribute *computed_attribute);
  virtual void item_removed (const std::string &item);
};


#endif // SVG_ITEM_OBSERVER_H
