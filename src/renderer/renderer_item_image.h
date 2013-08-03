#ifndef RENDERER_ITEM_IMAGE_H
#define RENDERER_ITEM_IMAGE_H

#include "renderer/renderer_graphics_item.h"

#include <QImage>

class QTransform;
enum class display;

class renderer_item_image : public renderer_graphics_item
{
  QPainterPath m_path;
  QRectF m_bbox;
  QRectF m_dst_rect;
  QRectF m_src_rect;
  QImage m_image_data;
  display m_display;

public:
  renderer_item_image (const std::string &name);
  ~renderer_item_image ();

  virtual void draw_graphics_item (SkCanvas &canvas, const renderer_state &state, const renderer_config *config) const override;
  virtual void update_bbox () override;

  void set_dest_rect (QRectF rect);
  void set_src_rect (QRectF rect);
  void set_source_rect (double x, double y, double w, double h);
  void set_image_data (QImage &image_data);
};


#endif // RENDERER_ITEM_IMAGE_H
