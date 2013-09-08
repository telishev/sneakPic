#ifndef SVG_ATTRIBUTE_CLIP_PATH_H
#define SVG_ATTRIBUTE_CLIP_PATH_H

#include "svg/attributes/abstract_attribute.h"
#include "svg/data_types/svg_data_type_iri.h"

class svg_item_clip_path;
class svg_items_container;

class svg_attribute_clip_path : public abstract_attribute
{
  SVG_ATTRIBUTE
  svg_data_type_iri m_iri;
public:
  svg_attribute_clip_path ();
  virtual ~svg_attribute_clip_path ();

  virtual bool read (const char *data, bool from_css = false) override;
  virtual bool write (QString &data, bool to_css = false) const override;

  const svg_item_clip_path *clip_path (const svg_items_container *container) const;
};

#endif // SVG_ATTRIBUTE_CLIP_PATH_H
