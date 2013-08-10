#ifndef SVG_ITEM_TEXT_H
#define SVG_ITEM_TEXT_H

#include "svg/items/svg_base_shape_item.h"

class QImage;
class QPainterPath;

class svg_item_text : public svg_base_shape_item
{
  SVG_ITEM

protected:
  double m_x, m_y; // For now we're supporting only single coordinates
  QString m_text;

public:
  svg_item_text (svg_document *document);
  virtual ~svg_item_text () override;

protected:
  virtual void item_read_complete () override;
  virtual bool check_item () override;
  virtual QPainterPath get_path () const override;

};

#endif // SVG_ITEM_TEXT_H
