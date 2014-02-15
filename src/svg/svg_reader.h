#ifndef SVG_OPENER_H
#define SVG_OPENER_H

class QString;
class QXmlStreamReader;

class abstract_svg_item;
class undo_handler;
class svg_item_factory;
class svg_document;
class svg_attribute_style;
class selectors_container;
class QXmlStreamReader;

enum class svg_attribute_type;

class svg_reader
{
  undo_handler     *m_undo_handler;
  svg_item_factory *m_item_factory;
  svg_document     *m_document;
  std::vector<abstract_svg_item *> m_root;

  selectors_container *m_selectors;
public:
  svg_reader (undo_handler *handler, svg_item_factory *item_factory, svg_document *document);
  ~svg_reader ();

  bool read_file (const QString &filename);
  bool read_steam (QXmlStreamReader &reader);
  abstract_svg_item *root () const;
  std::vector<abstract_svg_item *> all_roots () const { return m_root; }

  bool create_new_document ();

private:
  abstract_svg_item *process_new_item (QXmlStreamReader &reader, abstract_svg_item *cur_item);
  void add_style (abstract_svg_item *child_item, svg_attribute_style *style);
  void process_selectors (abstract_svg_item *root);
};

#endif // SVG_OPENER_H
