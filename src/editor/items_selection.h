#ifndef ITEMS_SELECTION_H
#define ITEMS_SELECTION_H

#include <set>
#include <QObject>
#ifdef _WINDOWS
#include <xutility>
#endif // _WINDOWS

class abstract_svg_item;
class svg_document;
class svg_items_container;
class QRectF;

class items_selection : public QObject
{
  Q_OBJECT

  typedef set<string> set_type;
  set_type m_selection;
  svg_items_container *m_container;
public:
  class selection_iterator : public std::iterator<std::forward_iterator_tag, abstract_svg_item *>
  {
    set_type::iterator m_it;
    svg_items_container *m_container;

  private:
    selection_iterator (svg_items_container *container, set_type::iterator it);

  public:
    abstract_svg_item *operator* ();
    bool operator != (const selection_iterator &) const;
    bool operator == (const selection_iterator &) const;
    selection_iterator &operator++();

    friend class items_selection;
  };

  items_selection (svg_items_container *container);
  ~items_selection ();

  bool empty () { return m_selection.empty (); }
  int count () const;
  const set_type &selection () const { return m_selection; }

  bool contains (const string &name) const;

  void add_item (const abstract_svg_item *item);
  void add_item (const string &item_name);

  void remove_item (const abstract_svg_item *item);
  void remove_item (const string &item_name);

  void clear ();

  void add_items_for_rect (const QRectF &rect, const abstract_svg_item *root);

  selection_iterator begin ();
  selection_iterator end ();

  void remove_unavailable_items ();


signals:
  void selection_changed ();
};

#endif // ITEMS_SELECTION_H
