#ifndef SVG_DOCUMENT_H
#define SVG_DOCUMENT_H

class svg_item_factory;
class svg_attribute_factory;
class svg_items_container;

class abstract_svg_item;
class svg_item_svg;
class renderer_items_container;
class abstract_renderer_event;
class rendered_items_cache;
class selectors_container;
class settings_t;
class events_queue;
class undoable_items_container_t;
class undo_handler;
class changed_items_container;
class QXmlStreamReader;

#include <QString>
#include <QObject>


class svg_document : public QObject
{
  Q_OBJECT

  svg_item_factory      *m_item_factory;
  svg_attribute_factory *m_attribute_factory;
  svg_items_container   *m_item_container;
  selectors_container   *m_selectors;
  settings_t            *m_settings;
  events_queue          *m_queue;
  undoable_items_container_t *m_undoable_items_container;
  undo_handler          *m_undo_handler;
  changed_items_container *m_changed_items;

  abstract_svg_item *m_root;
  svg_item_svg *m_item_svg;
  QString m_filename;
  int m_last_overlay_num;
  bool m_signals_enabled;

public:
  svg_document (settings_t *settings);
  ~svg_document ();

  svg_item_factory *item_factory () const { return m_item_factory; }
  svg_attribute_factory *attribute_factory () const { return m_attribute_factory; }
  svg_items_container *item_container () const { return m_item_container; }
  abstract_svg_item *root () const { return m_root; }
  selectors_container *selectors () const { return m_selectors; }

  settings_t *settings () const { return m_settings; }
  svg_item_svg *root_svg_item () const { return m_item_svg; }
  undoable_items_container_t *get_undoable_items_container () const { return m_undoable_items_container; }
  undo_handler *get_undo_handler () const { return m_undo_handler; }
  changed_items_container *changed_items () const { return m_changed_items; }


  bool get_doc_dimensions (double &width, double &height);

  bool read_file (const QString &filename_arg);
  bool write_file (const QString &filename);

  QString get_filename ();

  renderer_items_container *create_rendered_items (rendered_items_cache *cache);
  void create_renderer_item (renderer_items_container *renderer_items, abstract_svg_item *svg_item);

  QString create_overlay_name ();

  void set_queue (events_queue *queue) { m_queue = queue; }

  void apply_changes ();
  bool signals_enabled () const;

  void undo ();
  void redo ();

signals:
  void items_changed ();
private:
  abstract_svg_item *process_new_item (QXmlStreamReader &reader, abstract_svg_item *cur_item);
  void send_items_change ();
};

#endif // SVG_DOCUMENT_H

