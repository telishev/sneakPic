#ifndef DEPENDENT_ITEMS_GATHERER_H
#define DEPENDENT_ITEMS_GATHERER_H

class abstract_svg_item;
class svg_document;
class svg_data_type_iri;

class dependent_items_gatherer
{
  std::set<abstract_svg_item *> m_additional_items;
  svg_document *m_doc;
public:
  dependent_items_gatherer (svg_document *doc);
  ~dependent_items_gatherer ();

  void add_item (abstract_svg_item *item, bool add_to_additional = false);

  const std::set<abstract_svg_item *> &additional_items () const { return m_additional_items; }

private:
  void check_href (abstract_svg_item * item);
  void check_fill (abstract_svg_item * item);
  void check_stoke (abstract_svg_item * item);

  void check_iri (const svg_data_type_iri &iri);
};

#endif // DEPENDENT_ITEMS_GATHERER_H
