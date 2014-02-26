#ifndef SVG_CHARACTER_DATA_H
#define SVG_CHARACTER_DATA_H

#include "svg/items/abstract_svg_item.h"

#include "svg/items/svg_item_type.h"
#include "svg/svg_namespaces.h"

class svg_character_data : public abstract_svg_item
{
  string m_data;
public:
  svg_character_data (svg_document *document, const char *data)
    : abstract_svg_item (document)
  {
    m_data = data;
  }

  virtual ~svg_character_data () override {}
  virtual svg_item_type type () const override { return svg_item_type::CHARACTER_DATA; }

  virtual const char *type_name () const override { return ""; }
  virtual const char *namespace_uri () const override { return ""; }
  virtual const char *namespace_name () const override { return ""; }
  virtual svg_namespaces_t namespace_type () const override { return svg_namespaces_t::UNKNOWN; }

  const char *char_data () const { return m_data.c_str (); }
};

#endif // SVG_CHARACTER_DATA_H
