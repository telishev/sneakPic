#include "renderer/renderer_graphics_item.h"

#include "renderer/qt2skia.h"
#include "renderer/renderer_state.h"
#include "renderer/renderer_config.h"

#include "svg/attributes/svg_attributes_enum.h"

#include "skia/skia_includes.h"

renderer_graphics_item::renderer_graphics_item (const string &name)
  : abstract_renderer_item (name)
{
  m_has_clip_path = false;
  m_opacity = 1.0;
  m_display = display::INLINE;
  visible = true;
  m_ignore_bbox = false;
}

void renderer_graphics_item::set_visibility (bool visible_arg)
{
  visible = visible_arg;
}

void renderer_graphics_item::set_opacity (double opacity)
{
  m_opacity = opacity;
}

renderer_graphics_item::~renderer_graphics_item ()
{
}

void renderer_graphics_item::draw (SkCanvas &canvas, const renderer_state &state, const renderer_config *config) const
{
  if (!visible || m_display == display::NONE)
    return;

  QTransform item_transform = transform () * state.transform ();
  if (!m_ignore_bbox)
    {
      QRectF transformed_rect = state.transform ().mapRect (bounding_box_impl ());
      if (!state.rect ().intersects (transformed_rect.toRect ()))
        return;
    }

  canvas.save ();
  if (m_has_clip_path)
    {
      QPainterPath clip_path = item_transform.map (m_clip_path);
      canvas.clipPath (qt2skia::path (clip_path), SkRegion::kReplace_Op, !config->render_for_selection ());
    }
  canvas.setMatrix (qt2skia::matrix (item_transform));

  draw_graphics_item (canvas, state, config);
  canvas.restore ();
}
