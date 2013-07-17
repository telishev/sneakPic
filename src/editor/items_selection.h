#ifndef ITEMS_SELECTION_H
#define ITEMS_SELECTION_H

#include <string>
#include <set>

class abstract_svg_item;
class svg_document;

class items_selection
{
  typedef std::set<std::string> set_type;
  set_type m_selection;
  const svg_document *m_document;
public:
  items_selection (const svg_document *document);
  ~items_selection ();

  int selected_count () const;
  const set_type &selection () const { return m_selection; }

  void add_item (const abstract_svg_item *item);
  void add_item (const std::string &item_name);

  void clear ();
};

#endif // ITEMS_SELECTION_H
