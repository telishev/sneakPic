#ifndef SVG_DOCUMENT_H
#define SVG_DOCUMENT_H

class svg_item_factory;
class svg_attribute_factory;
class svg_items_container;

class abstract_svg_item;
class svg_item_svg;

class QString;
class QDomDocument;


class svg_document
{
  svg_item_factory      *m_item_factory;
  svg_attribute_factory *m_attribute_factory;
  svg_items_container   *m_item_container;

  abstract_svg_item *m_root;
  svg_item_svg *item_svg;
  QString *filename;
public:
  svg_document ();
  ~svg_document ();

  svg_item_factory *item_factory () const { return m_item_factory; }
  svg_attribute_factory *attribute_factory () const { return m_attribute_factory; }
  svg_items_container *item_container () const { return m_item_container; }
  abstract_svg_item *root () const { return m_root; }

  bool get_doc_dimensions (double &width, double &height);

  bool read_file (const QString &filename_arg);
  bool write_file (const QString &filename);

  void update_items ();

  QString get_filename ();
};

#endif // SVG_DOCUMENT_H

