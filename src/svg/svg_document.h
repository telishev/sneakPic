#ifndef SVG_DOCUMENT_H
#define SVG_DOCUMENT_H

class svg_item_factory;
class svg_attribute_factory;
class svg_named_items_container;

class abstract_svg_item;

class QString;
class QDomDocument;

class svg_document
{
  svg_item_factory             *m_item_factory;
  svg_attribute_factory        *m_attribute_factory;
  svg_named_items_container *m_item_container;

  abstract_svg_item *m_root;
public:
  svg_document ();
  ~svg_document ();

  svg_item_factory *item_factory () const { return m_item_factory; }
  svg_attribute_factory *attribute_factory () const { return m_attribute_factory; }
  svg_named_items_container *item_container () const { return m_item_container; }
  abstract_svg_item *root () const { return m_root; }

  bool read_file (const QString &filename);
  bool write_file (const QString &filename);
};

#endif // SVG_DOCUMENT_H

