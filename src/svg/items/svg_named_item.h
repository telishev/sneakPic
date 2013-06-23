#ifndef SVG_NAMED_item_H
#define SVG_NAMED_item_H

#include "svg/items/abstract_svg_item.h"

#include <QString>

class svg_attribute_id;

class svg_named_item : public abstract_svg_item
{
  svg_attribute_id *m_id;
public:
  svg_named_item (svg_document *document);
  ~svg_named_item ();

  bool has_name () const { return m_id != nullptr; }
  QString name () const;
  void set_name (const QString &name);

protected:
  virtual void process_attribute (abstract_attribute *attribute) override;

private:
  void add_to_container ();
  void remove_from_container ();
};

#endif // SVG_NAMED_item_H
