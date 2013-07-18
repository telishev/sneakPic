#include "renderer/overlay_renderer.h"

#include "common/memory_deallocation.h"

#include "editor/items_selection.h"

#include "renderer/renderer_items_container.h"
#include "renderer/svg_renderer.h"
#include "renderer/renderer_state.h"
#include "renderer/renderer_config.h"
#include "renderer/qt2skia.h"
#include "renderer/renderer_overlay_root.h"
#include "renderer/renderer_page.h"
#include "renderer/overlay_item_type.h"
#include "renderer/rendered_items_cache.h"

#include "svg/svg_document.h"
#include "svg/items/svg_items_container.h"
#include "svg/items/abstract_svg_item.h"

#pragma warning(push, 0)
#include <SkCanvas.h>
#include <SkSurface.h>
#include <SkDevice.h>
#pragma warning(pop)

#include <QRect>
#include <QTransform>
#include <QImage>
#include <QPainter>
#include <memory>



overlay_renderer::overlay_renderer (rendered_items_cache *cache)
{
  m_document = nullptr;
  m_container = nullptr;
  m_renderer = new svg_renderer (nullptr, nullptr);
  m_root = nullptr;
  m_page_item = nullptr;
  m_cache = cache;
}

overlay_renderer::~overlay_renderer ()
{
  FREE (m_container);
  FREE (m_renderer);
}

void overlay_renderer::set_document (svg_document *document)
{
  FREE (m_container);
  m_container = new renderer_items_container;
  m_root = new renderer_overlay_root ("#overlay_root");
  m_container->add_item (m_root);
  m_container->set_root (m_root->name ());
  m_document = document;
  m_current_item = std::string ();

  m_page_item = new renderer_page ("#page_item");
  double width, height;
  m_document->get_doc_dimensions (width, height);
  m_page_item->set_height (height);
  m_page_item->set_width (width);
}

void overlay_renderer::set_current_item (const std::string &id)
{
  if (!m_current_item.empty ())
    {
      m_root->erase_child (m_current_item);
      m_container->remove_item (m_current_item);
      m_current_item = std::string ();
    }

  if (id.empty ())
    return;

  m_current_item = add_item (id, overlay_item_type::CURRENT_ITEM);
}

void overlay_renderer::selection_changed (const items_selection *selection)
{
  for (const std::string &item : m_selection)
    {
      m_root->erase_child (item);
      m_container->remove_item (item);
    }

  m_selection.clear ();

  for (const std::string &item : selection->selection ())
    {
      std::string new_item = add_item (item, overlay_item_type::SELECTION);
      if (!new_item.empty ())
        m_selection.insert (new_item);
    }
}


void overlay_renderer::draw (QPainter &painter, const QRect &rect_to_draw, const QTransform &transform)
{
  if (!m_container)
    return;

  SkBitmap bitmap;
  bitmap.setConfig (SkBitmap::kARGB_8888_Config, rect_to_draw.width (), rect_to_draw.height ());
  bitmap.allocPixels ();
  bitmap.eraseColor (SK_ColorWHITE);
  SkDevice device (bitmap);
  SkCanvas canvas (&device);

  m_cache->lock ();
  canvas.drawBitmap (m_cache->get_current_screen (), 0, 0);
  m_cache->unlock ();

  m_renderer->draw_to_bitmap (rect_to_draw, transform, m_container->root (), &bitmap);
  QImage img = qt2skia::qimage (bitmap);
  painter.drawImage (rect_to_draw, img);
}

void overlay_renderer::draw_page (QPainter &painter, const QRect &rect_to_draw, const QTransform &transform)
{
  if (!m_page_item)
    return;

  std::unique_ptr<SkBitmap> bitmap (m_renderer->draw_to_bitmap (rect_to_draw, transform, m_page_item));
  QImage img = qt2skia::qimage (*bitmap);
  painter.drawImage (rect_to_draw, img);
}

std::string overlay_renderer::add_item (const std::string &name, overlay_item_type type)
{
  abstract_svg_item *svg_item = m_document->item_container ()->get_item (QString::fromStdString (name));
  if (!svg_item)
    return std::string ();

  abstract_renderer_item *overlay_item = svg_item->create_overlay_item (type);
  if (!overlay_item)
    return std::string ();

  m_container->add_item (overlay_item);
  m_container->add_child (m_root->name (), overlay_item->name ());
  return overlay_item->name ();
}
