#include "renderer/overlay_renderer.h"

#include "common/memory_deallocation.h"

#include "renderer/renderer_items_container.h"
#include "renderer/svg_renderer.h"
#include "renderer/renderer_state.h"
#include "renderer/renderer_config.h"
#include "renderer/qt2skia.h"
#include "renderer/renderer_overlay_root.h"


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


overlay_renderer::overlay_renderer ()
{
  m_document = nullptr;
  m_container = nullptr;
  m_renderer = new svg_renderer (nullptr, nullptr);
  m_root = nullptr;
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

  abstract_svg_item *svg_item = m_document->item_container ()->get_item (QString::fromStdString (id));
  if (!svg_item)
    return;

  abstract_renderer_item *overlay_item = svg_item->create_overlay_item ();
  if (!svg_item)
    return;

  m_container->add_item (overlay_item);
  m_container->add_child (m_root->name (), overlay_item->name ());
  m_current_item = overlay_item->name ();
}

void overlay_renderer::draw (QPainter &painter, const QRect &rect_to_draw, const QTransform &transform)
{
  if (!m_container)
    return;
  SkBitmap bitmap;
  bitmap.setConfig (SkBitmap::kARGB_8888_Config, rect_to_draw.width (), rect_to_draw.height ());
  bitmap.allocPixels ();
  SkDevice device (bitmap);
  SkCanvas canvas (&device);
  canvas.drawColor (SK_ColorTRANSPARENT, SkXfermode::kSrc_Mode);

  svg_renderer renderer (nullptr, nullptr);
  renderer_state state (rect_to_draw, transform);
  renderer_config cfg;
  renderer.draw_item (m_container->root (), canvas, state, cfg);
  QImage img = qt2skia::qimage (bitmap);
  painter.drawImage (rect_to_draw, img);
}
