#ifndef SVG_ATTRIBUTE_CLIP_PATH_H
#define SVG_ATTRIBUTE_CLIP_PATH_H

#include "svg/attributes/svg_iri_attributes.h"

class svg_item_clip_path;
class svg_items_container;

class svg_attribute_clip_path : public svg_iri_attribute
{
  SVG_ATTRIBUTE

public:
  svg_attribute_clip_path ();
  virtual ~svg_attribute_clip_path ();

  virtual bool read (const char *data, bool from_css = false) override;
  virtual bool write (QString &data, bool to_css = false) const override;

  const svg_item_clip_path *clip_path (const svg_items_container *container) const;
  void set_clip_path (svg_item_clip_path *item);
};

#endif // SVG_ATTRIBUTE_CLIP_PATH_H
