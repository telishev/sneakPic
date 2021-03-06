#include "renderer/renderer_base_shape_item.h"

#include "common/memory_deallocation.h"
#include "common/debug_utils.h"
#include "common/math_defs.h"

#include <QPainter>

#include "renderer_paint_server.h"
#include "renderer/qt2skia.h"
#include "renderer/rendered_items_cache.h"
#include "renderer/renderer_config.h"
#include "renderer/renderer_items_container.h"
#include "renderer/renderer_state.h"

#include "skia/skia_includes.h"

renderer_base_shape_item::renderer_base_shape_item (const string &name)
  : renderer_graphics_item (name)
{
  m_stroke = new SkPaint;
  m_stroke->setStyle (SkPaint::kStroke_Style);
  m_fill = new SkPaint;
  m_fill->setStyle (SkPaint::kFill_Style);
  m_stroke->setAntiAlias (true);
  m_fill->setAntiAlias (true);
  m_stroke_server = nullptr;
  m_fill_server = nullptr;
}

void renderer_base_shape_item::add_marker (abstract_renderer_item *marker)
{
  marker_renderer_items.push_back (marker);
}

renderer_base_shape_item::~renderer_base_shape_item ()
{
  FREE (m_stroke);
  FREE (m_fill);
  FREE (m_stroke_server);
  FREE (m_fill_server);
}

void renderer_base_shape_item::set_stroke_linecap (Qt::PenCapStyle linecap)
{
  switch  (linecap)
    {
    case Qt::FlatCap: m_stroke->setStrokeCap (SkPaint::kButt_Cap); break;
    case Qt::RoundCap: m_stroke->setStrokeCap (SkPaint::kRound_Cap); break;
    case Qt::SquareCap: m_stroke->setStrokeCap (SkPaint::kSquare_Cap); break;
    default:
      return;
    }

}

void renderer_base_shape_item::set_stroke_linejoin (Qt::PenJoinStyle linejoin)
{
  switch (linejoin)
    {
    case Qt::BevelJoin: m_stroke->setStrokeJoin (SkPaint::kBevel_Join); break;
    case Qt::SvgMiterJoin: m_stroke->setStrokeJoin (SkPaint::kMiter_Join); break;
    case Qt::RoundJoin: m_stroke->setStrokeJoin (SkPaint::kRound_Join); break;
    default:
      return;
    }
}

void renderer_base_shape_item::set_stroke_miterlimit (double miterlimit)
{
  m_stroke->setStrokeMiter (SkFloatToScalar (miterlimit));
}

void renderer_base_shape_item::set_dash_array (QList<double> dash_array, double offset)
{
  if (dash_array.size () == 0)
    return;

  SkScalar *dash_offsets = new SkScalar [dash_array.size ()];
  for (int i = 0; i < dash_array.size (); i++)
    {
      dash_offsets[i] = SkFloatToScalar (dash_array[i]);
    }
  SkDashPathEffect *effect = SkDashPathEffect::Create (dash_offsets, dash_array.size(), SkFloatToScalar(offset));
  m_stroke->setPathEffect (effect)->unref ();
  FREE_ARRAY (dash_offsets);
}

void renderer_base_shape_item::set_stroke_width (double width)
{
  m_stroke->setStrokeWidth (SkFloatToScalar (width));
}

bool renderer_base_shape_item::configure_painter (SkPaint &paint, bool stroke, bool config_for_selection) const
{
  if (stroke)
    {
      paint = *m_stroke;
      if (m_stroke_server)
        m_stroke_server->fill_paint (paint, transform ().inverted ().mapRect (m_bbox_computed));
    }
  else
    {
      paint = *m_fill;
      if (m_fill_server)
        m_fill_server->fill_paint (paint, transform ().inverted ().mapRect (m_bbox_computed));
    }

  if (SkColorGetA (paint.getColor ()) == 0)
    return false;

  if (config_for_selection)
    return configure_painter_for_selection (paint);
  else
    return true;
}

void renderer_base_shape_item::set_stroke_server (const renderer_paint_server *server)
{
  FREE (m_stroke_server);
  m_stroke_server = server->clone ();
}

void renderer_base_shape_item::set_fill_server (const renderer_paint_server *server)
{
  FREE (m_fill_server);
  m_fill_server = server->clone ();
}

void renderer_base_shape_item::update_bbox_impl ()
{
  m_bbox_computed = m_bbox;
  for (size_t i = 0; i < marker_renderer_items.size (); i++)
    {
      marker_renderer_items[i]->update_bbox ();
      m_bbox_computed = m_bbox_computed.united (marker_renderer_items[i]->bounding_box ());
    }
}

void renderer_base_shape_item::draw_graphics_item (SkCanvas &canvas, const renderer_state &state, const renderer_config *config)  const
{
  SkPath path = qt2skia::path (m_path);
  for (int i = 0; i < 2; i++)
    {
      SkPaint paint;
      configure_painter (paint, i != 0, config->render_for_selection ());
      canvas.drawPath (path, paint);
    }

  for (size_t i = 0; i < marker_renderer_items.size (); i++)
  {
    canvas.restore ();
    SkPaint paint;
    marker_renderer_items[i]->draw (canvas, state, config);
    canvas.save ();
  }
}

