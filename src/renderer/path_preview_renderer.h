#ifndef PATH_PREVIEW_RENDERER_H
#define PATH_PREVIEW_RENDERER_H

#include "renderer/renderable_item.h"

#include <QColor>

class svg_path;

class path_preview_renderer : public renderable_item
{
  const svg_path *m_path;
  QColor m_color;
public:
  path_preview_renderer (const svg_path *path, QColor color);
  ~path_preview_renderer ();

  void set_path (const svg_path *path);
  void set_color (QColor color);
  virtual void draw (SkCanvas &canvas, const renderer_state &state, const renderer_config *config) const override;
};

#endif // PATH_PREVIEW_RENDERER_H
