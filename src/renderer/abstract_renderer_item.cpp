#include "renderer/abstract_renderer_item.h"

#include "renderer/renderer_items_container.h"
#include "renderer/rendered_items_cache.h"
#include "renderer/qt2skia.h"

#include "common/debug_utils.h"

#include <QColor>

#pragma warning(push, 0)
#include <SkPaint.h>
#pragma warning(pop)


abstract_renderer_item::abstract_renderer_item (const std::string &name)
{
  m_name = name;
  m_container = nullptr;
  m_unique_id = -1;
}

abstract_renderer_item::~abstract_renderer_item ()
{
}

void abstract_renderer_item::push_back_child (const std::string &child)
{
  m_children.push_back (child);
}

void abstract_renderer_item::erase_child (const std::string &child)
{
  m_children.erase (std::remove (m_children.begin (), m_children.end (), child), m_children.end ());
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
