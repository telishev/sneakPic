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

  typedef set<string> set_type;
  set_type m_selection;
  svg_items_container *m_container;
  mutable bool m_bbox_invalid; // for caching in get_bbox
  mutable QRectF m_bbox;       // ditto
public:
  class iterator : public std::iterator<std::forward_iterator_tag, abstract_svg_item *>
  {
    set_type::iterator m_it;
    svg_items_container *m_container;

  private:
    iterator (svg_items_container *container, set_type::iterator it) { m_container = container; m_it = it; }

  public:
    abstract_svg_item *operator* ();
    bool operator != (const iterator &other) const {return (this->m_it != other.m_it);}
    bool operator == (const iterator &other) const {return (this->m_it == other.m_it);}
    iterator &operator++() {this->m_it++; return *this;}

    friend class items_selection;
  };

  class const_iterator : public std::iterator<std::forward_iterator_tag, abstract_svg_item *>
  {
    set_type::const_iterator m_it;
    const svg_items_container *m_container;

  private:
    const_iterator (const svg_items_container *container, set_type::const_iterator it) { m_container = container; m_it = it; }

  public:
    abstract_svg_item *operator* ();
    bool operator != (const const_iterator &other) const {return (this->m_it != other.m_it);}
    bool operator == (const const_iterator &other) const {return (this->m_it == other.m_it);}
    const_iterator &operator++() {this->m_it++; return *this;}

    friend class items_selection;
  };

  items_selection (svg_items_container *container);
  ~items_selection ();

  abstract_svg_item *single_item () const ;

  bool empty () { return m_selection.empty (); }
  int count () const;
  const set_type &selection () const { return m_selection; }

  bool contains (const string &name) const;

  void add_item (const abstract_svg_item *item);
  void add_item (const string &item_name);

  void remove_item (const abstract_svg_item *item);
  void remove_item (const string &item_name);

  void clear ();

  QRectF get_bbox () const;

  void add_items_for_rect (const QRectF &rect, const abstract_svg_item *root);

  iterator begin () { return iterator (m_container, m_selection.begin ()); }
  iterator end () { return iterator (m_container, m_selection.end ()); }
  const_iterator begin () const { return const_iterator (m_container, m_selection.begin ()); }
  const_iterator end () const { return const_iterator (m_container, m_selection.end ()) ;}

  void remove_unavailable_items ();

  svg_items_container *container () const { return m_container; }


signals:
  void selection_changed ();
  };

#endif // ITEMS_SELECTION_H
