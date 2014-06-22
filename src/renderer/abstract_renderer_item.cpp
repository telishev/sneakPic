#include "renderer/abstract_renderer_item.h"

#include "renderer/renderer_items_container.h"
#include "renderer/rendered_items_cache.h"
#include "renderer/qt2skia.h"

#include "common/debug_utils.h"

#include <QColor>

#include "skia/skia_includes.h"
#include "common/range_algorithm.h"


abstract_renderer_item::abstract_renderer_item (const string &name)
{
  m_name = name;
  m_container = nullptr;
  m_unique_id = -1;
  m_bbox_valid = false;
}

abstract_renderer_item::~abstract_renderer_item ()
{
}

void abstract_renderer_item::push_back_child (const string &child)
{
  m_children.push_back (child);
}

void abstract_renderer_item::erase_child (const string &child)
{
  range::erase (m_children, child);
}

bool abstract_renderer_item::configure_painter_for_selection (SkPaint &paint) const
{
  DEBUG_ASSERT (m_unique_id < (1 << 24));
  paint.setAntiAlias (false);
  QColor color = rendered_items_cache::get_selection_color (m_unique_id);
  paint.setColor (qt2skia::color (color));
  paint.setShader (nullptr);
  return true;
}

QRectF abstract_renderer_item::bounding_box () const
{
  return bounding_box_impl ();
}

void abstract_renderer_item::update_bbox ()
{
  if (m_bbox_valid)
    return;

  update_bbox_impl ();
  m_bbox_valid = true;
}

void abstract_renderer_item::invalidate_bbox ()
{
  m_bbox_valid = false;
  if (!m_container)
    return;

  for (abstract_renderer_item *cur_item = this; cur_item; cur_item = cur_item->parent ())
    cur_item->m_bbox_valid = false;
}

abstract_renderer_item *abstract_renderer_item::parent () const
{
  return m_container->item (m_parent);
}
