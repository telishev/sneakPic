#ifndef RENDERER_ITEM_IMAGE_H
#define RENDERER_ITEM_IMAGE_H

#include "renderer/renderer_base_shape_item.h"

#include <QImage>

class QTransform;

class renderer_item_image : public abstract_renderer_item
{
  QPainterPath m_path;
  QRectF m_bbox;
  double m_x, m_y, m_width, m_height;
  QImage m_image_data;

public:
  renderer_item_image (const std::string &name);
  ~renderer_item_image ();

  virtual void draw (SkCanvas &canvas, const renderer_state &state, const renderer_config *config) const override;
  virtual QRectF bounding_box () const override { return m_bbox; }
  virtual void update_bbox () override;

  void set_dimensions (double x, double y, double w, double h);
  void set_image_data (QImage &image_data);
};


#endif // RENDERER_ITEM_IMAGE_H
