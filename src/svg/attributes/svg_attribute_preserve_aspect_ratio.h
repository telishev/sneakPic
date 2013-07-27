#ifndef SVG_ATTRIBUTE_PRESERVE_RATIO_H
#define SVG_ATTRIBUTE_PRESERVE_RATIO_H

#include "svg/attributes/abstract_attribute.h"

class QRectF;
class QString;

enum class perserve_aspect_ratio_align
{
  NONE = 0,
  MIN,
  MID,
  MAX,

  INVALID
};

enum class perserve_aspect_ratio_behaviour
{
  MEET = 0,
  SLICE,

  INVALID
};

class svg_attribute_preserve_aspect_ratio : public abstract_attribute
{
  SVG_ATTRIBUTE

  bool m_defer;
  perserve_aspect_ratio_align m_x_align, m_y_align;
  perserve_aspect_ratio_behaviour m_behaviour;

public:
  svg_attribute_preserve_aspect_ratio (abstract_svg_item *item);
  virtual ~svg_attribute_preserve_aspect_ratio ();

  // returned rectangle is the destination rectangle for rendering, original_rect can be changed (cut) by function
  QRectF get_desired_rect (QRectF &original_rect, QRectF viewport_rect) const;

  virtual bool read (const char *data, bool from_css = false) override;
  virtual bool write (QString &data, bool to_css = false) const override;
};
#endif // SVG_ATTRIBUTE_PRESERVE_RATIO_H
