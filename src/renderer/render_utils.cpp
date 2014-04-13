#include "renderer/render_utils.h"

#include "renderer/renderer_items_container.h"
#include "renderer/abstract_renderer_item.h"
#include "renderer/svg_renderer.h"
#include "renderer/qt2skia.h"

#include "svg/svg_document.h"

#include "skia/skia_includes.h"

#include <QImage>
#include <QTransform>

namespace render_utils
{

void render_to_image (svg_document &doc, QString file_name, int width, int height)
{
  QRect rect = QRect (0, 0, width, height);
  unique_ptr<renderer_items_container> container (doc.create_rendered_items (nullptr));

  svg_renderer renderer (nullptr, nullptr);
  unique_ptr<SkBitmap> bitmap (renderer.draw_to_bitmap (rect, QTransform (), container->root ()));
  qt2skia::qimage (*bitmap).save (file_name);
}
}
