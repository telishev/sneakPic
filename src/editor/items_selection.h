#ifndef ITEMS_SELECTION_H
#define ITEMS_SELECTION_H

#include <set>
#include <QObject>

class abstract_svg_item;
class svg_document;
class svg_items_container;
class QRectF;

class items_selection : public QObject
{
  Q_OBJECT

  typedef std::set<std::string> set_type;
  set_type m_selection;
  svg_items_container *m_container;
public:
  class iterator
  {
    set_type::iterator m_it;
    svg_items_container *m_container;

  private:
    iterator (svg_items_container *container, set_type::iterator it);

  public:
    abstract_svg_item *operator* ();
    bool operator != (const iterator &) const;
    iterator &operator++();

    friend class items_selection;
  };

  items_selection (svg_items_container *container);
  ~items_selection ();

  bool empty () { return m_selection.empty (); }
  int count () const;
  const set_type &selection () const { return m_selection; }

  bool contains (const std::string &name) const;

  void add_item (const abstract_svg_item *item);
  void add_item (const std::string &item_name);

  void remove_item (const abstract_svg_item *item);
  void remove_item (const std::string &item_name);

  void clear ();

  void add_items_for_rect (const QRectF &rect, const abstract_svg_item *root);

  iterator begin ();
  iterator end ();

  void remove_unavailable_items ();


signals:
  void selection_changed ();
};

#endif // ITEMS_SELECTION_H
