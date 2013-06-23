#ifndef SVG_ITEM_SVG_H
#define SVG_ITEM_SVG_H

#include "svg/items/svg_named_item.h"
#include "svg/svg_namespaces.h"

#include <QString>

class svg_item_svg : public svg_named_item
{
  SVG_ITEM
public:
  svg_item_svg (svg_document *document);
  virtual ~svg_item_svg () override;
  virtual item_type type () const override { return item_type::SVG; }
  static QString static_name () { return "svg"; }

  static svg_namespaces_t static_ns_type () { return svg_namespaces_t::SVG; }

protected:
  virtual void post_read () override;

  

};

#endif // SVG_ITEM_SVG_H
