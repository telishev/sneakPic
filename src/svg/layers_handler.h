#ifndef LAYERS_HANDLER_H
#define LAYERS_HANDLER_H

#include <QObject>

class abstract_svg_item;
class svg_document;
class svg_items_container;

class layers_handler : public QObject
{
  Q_OBJECT

  typedef deque<string> container_type;
  container_type m_layers_container; // contains names of "layer" items (groups with special attribute)
  svg_document *m_document;
  int m_active_layer_index;
  abstract_svg_item *get_layer_item (int index) const;

public:
  class layers_iterator : public std::iterator<std::forward_iterator_tag, abstract_svg_item *>
  {
    container_type::iterator m_it;
    svg_items_container *m_container;

  private:
    layers_iterator (svg_items_container *container, container_type::iterator it);

  public:
    abstract_svg_item *operator* ();
    bool operator != (const layers_iterator &other) const;
    bool operator == (const layers_iterator &other) const;
    int operator - (const layers_iterator &other) const;
    layers_iterator operator + (int add) const;
    layers_iterator &operator++();

    friend class layers_handler;
  };

  layers_handler (svg_document *document);
  ~layers_handler ();
  QString get_layer_name (int index) const;
  layers_iterator begin ();
  layers_iterator end ();
  int layers_count();
  int active_layer_index ();
  abstract_svg_item *get_active_layer_item () const;
  bool is_layer_visible( int index );
  void toggle_layer_visibility (int layer_index);
  void rename_layer(int index, QString new_name);
  void move_layer (int from, int to);
  int get_active_layer_opacity ();
  int is_layer_selected ();
  void set_active_layer_opacity (int value);

private:
  void update_layer_list ();
  void update_active_layer_index_by_attribute();
  void update_attribute_by_active_layer_index ();
  void add_new_layer (QString name = QString ());

public slots:
  void set_active_layer (int new_index );
  void add_new_layer_slot ();
  void remove_active_layer ();

private slots:
  void on_undo_redo_done ();

signals:
  void layers_changed (); // for non-obvious cases (like undo)
  void active_layer_changed ();
  };

#endif //LAYERS_HANDLER_H
