#ifndef SVG_DOCUMENT_H
#define SVG_DOCUMENT_H

class svg_item_factory;
class svg_attribute_factory;
class svg_items_container;

class abstract_svg_item;
class svg_item_svg;
class renderer_items_container;
class abstract_renderer_event;

template<typename T>
class wait_queue;

class QDomDocument;

#include <QString>


class svg_document
{
  svg_item_factory      *m_item_factory;
  svg_attribute_factory *m_attribute_factory;
  svg_items_container   *m_item_container;

  abstract_svg_item *m_root;
  svg_item_svg *item_svg;
  QString filename;

  wait_queue<abstract_renderer_event> *m_queue;
public:
  svg_document (wait_queue<abstract_renderer_event> *queue = nullptr);
  ~svg_document ();

  svg_item_factory *item_factory () const { return m_item_factory; }
  svg_attribute_factory *attribute_factory () const { return m_attribute_factory; }
  svg_items_container *item_container () const { return m_item_container; }
  abstract_svg_item *root () const { return m_root; }

  bool get_doc_dimensions (double &width, double &height);

  bool read_file (const QString &filename_arg);
  bool write_file (const QString &filename);

  QString get_filename ();

  renderer_items_container *create_rendered_items ();
  void create_renderer_item (renderer_items_container *renderer_items, abstract_svg_item *svg_item);
};

#endif // SVG_DOCUMENT_H

