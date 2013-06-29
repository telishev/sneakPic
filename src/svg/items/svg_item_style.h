#ifndef SVG_ITEM_STYLE_H
#define SVG_ITEM_STYLE_H

#include "svg/items/abstract_svg_item.h"

#include <vector>

class css_declaration;
class abstract_css_selector;

class svg_item_style : public abstract_svg_item
{
  SVG_ITEM

  std::vector<std::pair <abstract_css_selector *, css_declaration *>> m_rule_set;
public:
  svg_item_style (svg_document *document);
  virtual ~svg_item_style () override;

  /// finds attribute from the matching selector
  const abstract_attribute *get_attribute (const std::string &str, const abstract_svg_item *item_to_match) const;

protected:
  virtual bool read_item (const QString &data) override;

private:
  bool read_item (const char *data);

};

#endif // SVG_ITEM_STYLE_H
