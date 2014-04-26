#include "dependent_items_gatherer.h"

#include "svg/items/abstract_svg_item.h"
#include "svg/attributes/svg_attribute_xlink_href.h"
#include "svg/attributes/svg_attributes_fill_stroke.h"
#include "svg/svg_document.h"
#include "svg/items/svg_items_container.h"




dependent_items_gatherer::dependent_items_gatherer (svg_document *doc)
{
  m_doc = doc;
}

dependent_items_gatherer::~dependent_items_gatherer ()
{

}

void dependent_items_gatherer::add_item (abstract_svg_item *item, bool add_to_additional)
{
  if (!item || m_additional_items.count (item) != 0)
    return;

  check_href (item);
  check_fill (item);
  check_stoke (item);

  if (add_to_additional)
    m_additional_items.insert (item);
}

void dependent_items_gatherer::check_href (abstract_svg_item * item)
{
  auto href = item->get_computed_attribute<svg_attribute_xlink_href> ();
  check_iri (href->iri ());
}

void dependent_items_gatherer::check_fill (abstract_svg_item * item)
{
  auto fill = item->get_computed_attribute<svg_attribute_fill> ();
  check_iri (fill->iri ());
}

void dependent_items_gatherer::check_stoke (abstract_svg_item * item)
{
  auto stroke = item->get_computed_attribute<svg_attribute_stroke> ();
  check_iri (stroke->iri ());
}

void dependent_items_gatherer::check_iri (const svg_data_type_iri &iri)
{
  add_item (m_doc->item_container ()->get_item (iri.get_fragment_name ()), true);
}
