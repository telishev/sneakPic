#include "renderer/renderer_page.h"

#include "renderer/renderer_state.h"
#include "renderer/qt2skia.h"
#include "renderer/renderer_config.h"

#pragma warning(push, 0)
#include <SkCanvas.h>
#pragma warning(pop)


renderer_page::renderer_page (double width, double height)
{
  m_height = height;
  m_width  = width;
}

renderer_page::~renderer_page ()
{

}

void renderer_page::draw (SkCanvas &canvas, const renderer_state &state, const renderer_config *config) const 
{
  canvas.setMatrix (qt2skia::matrix (state.transform ()));

  if (!config->render_for_selection ())
    {
      SkPaint paint;
      paint.setStyle (SkPaint::kStroke_Style);
      double stroke_width = 2.0 / qMax (state.transform ().m11 (), state.transform ().m22 ());
      paint.setStrokeWidth (stroke_width);
      canvas.drawRect (SkRect::MakeXYWH (SkFloatToScalar (0.0), SkFloatToScalar (0.0), SkFloatToScalar (m_width), SkFloatToScalar (m_height)), paint);
    }
}

