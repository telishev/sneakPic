#ifndef SVG_DOCUMENT_H
#define SVG_DOCUMENT_H

class svg_item_factory;
class svg_items_container;

class abstract_svg_item;
class svg_item_svg;
class renderer_items_container;
class abstract_renderer_event;
class rendered_items_cache;
class selectors_container;
class settings_t;
class events_queue;
class undo_handler;
class items_edit_handler_t;
class QXmlStreamReader;

#include <QString>
#include <QObject>


class svg_document : public QObject
{
  Q_OBJECT

  svg_item_factory      *m_item_factory;
  svg_items_container   *m_item_container;
  events_queue          *m_queue;
  items_edit_handler_t  *m_items_edit_handler;

  abstract_svg_item *m_root;
  QString m_filename;
  int m_last_overlay_num;
  bool m_signals_enabled;

public:
  svg_document ();
  ~svg_document ();

  svg_item_factory *item_factory () const { return m_item_factory; }
  svg_items_container *item_container () const { return m_item_container; }
  abstract_svg_item *root () const { return m_root; }

  undo_handler *get_undo_handler () const;
  items_edit_handler_t *items_edit_handler () const { return m_items_edit_handler; }

  bool read_file (const QString &filename_arg);
  bool write_file (const QString &filename);

  QString get_filename ();

  renderer_items_container *create_rendered_items (rendered_items_cache *cache);
  void create_renderer_item (renderer_items_container *renderer_items, abstract_svg_item *svg_item);

  std::string create_overlay_name ();

  void set_queue (events_queue *queue) { m_queue = queue; }

  void apply_changes ();
  bool signals_enabled () const;
  void set_signals_enabled (bool enable);

  void undo ();
  void redo ();

signals:
  void items_changed ();
private:
  void send_items_change ();
};

#endif // SVG_DOCUMENT_H

