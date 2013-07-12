#include "renderer/renderer_item_svg.h"

#include <QPainter>

#include "renderer/renderer_state.h"
#include "renderer/qt2skia.h"
#include "renderer/renderer_config.h"

#pragma warning(push, 0)
#include <SkCanvas.h>
#include <SkSurface.h>
#include <SkDevice.h>
#pragma warning(pop)


renderer_item_svg::renderer_item_svg (const std::string &name)
  : renderer_item_group (name)
{
  m_height = m_width = 0.0;

}

renderer_item_svg::~renderer_item_svg ()
{

}

void renderer_item_svg::draw (SkCanvas &canvas, const renderer_state &state, const renderer_config *config) const
{
  /// TODO: render document boundaries

  canvas.setMatrix (qt2skia::matrix (state.transform ()));

  if (!config->render_for_selection ())
    {
      SkPaint paint;
      paint.setStyle (SkPaint::kStroke_Style);
      canvas.drawRect (SkRect::MakeXYWH (SkFloatToScalar (0.0), SkFloatToScalar (0.0), SkFloatToScalar (m_width), SkFloatToScalar (m_height)), paint);
    }

  renderer_item_group::draw (canvas, state, config);
}

void renderer_item_svg::set_height (double height)
{
  m_height = height;
}

void renderer_item_svg::set_width (double width)
{
  m_width = width;
}

