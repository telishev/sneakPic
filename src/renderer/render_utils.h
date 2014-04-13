#ifndef RENDER_UTILS_H
#define RENDER_UTILS_H

class svg_document;

namespace render_utils
{
  void render_to_image (svg_document &doc, QString file_name, int width, int height);
}
#endif // RENDER_UTILS_H

