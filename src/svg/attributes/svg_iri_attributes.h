#ifndef SVG_IRI_ATTRIBUTES_H
#define SVG_IRI_ATTRIBUTES_H

#include "abstract_attribute.h"
#include "svg/data_types/svg_data_type_iri.h"

class svg_iri_attribute : public abstract_attribute
{
protected:
  svg_data_type_iri m_iri;
public:

  const svg_data_type_iri &iri () const { return m_iri; };
  void set_iri (const svg_data_type_iri &iri);

protected:
  void update_observers ();
  void remove_observers ();
};

#endif // SVG_IRI_ATTRIBUTES_H
