#ifndef PATH_PREVIEW_RENDERER_H
#define PATH_PREVIEW_RENDERER_H

#include "renderer/renderable_item.h"
#include "skia/skia_includes.h"

#include <QColor>

class svg_path_geom;

class simple_path_renderer : public renderable_item
{
  SkPath m_path;
  QColor m_color;
public:
  simple_path_renderer (SkPath path, QColor color);
  ~simple_path_renderer ();

  void set_simple_path (SkPath path);
  void set_color (QColor color);

  virtual void draw (SkCanvas &canvas, const renderer_state &state, const renderer_config *config) const override;
  virtual SkPath get_path () const;
};

class path_preview_renderer : public simple_path_renderer
{
  const svg_path_geom *m_path;  
public:
  path_preview_renderer (const svg_path_geom *path, QColor color);
  ~path_preview_renderer ();

  void set_path (const svg_path_geom *path);
  virtual SkPath get_path () const override;
};

#endif // PATH_PREVIEW_RENDERER_H
