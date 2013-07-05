#include "renderer/renderer_item_group.h"

#include "svg/items/abstract_svg_item.h"

#include "renderer/renderer_state.h"

#include <QPixmap>
#include <QPainter>

renderer_item_group::renderer_item_group (const abstract_svg_item *svg_item)
{
  m_opacity = 1.0;
  m_bbox_calculated = false;
  m_svg_item = svg_item;
  m_has_clip_path = false;
}

renderer_item_group::~renderer_item_group ()
{

}

void renderer_item_group::draw (QPainter &painter, const renderer_state &state) const
{
  if (!m_svg_item)
    return;

  QTransform item_transform = transform () * state.transform ();
  QRectF transformed_rect = state.transform ().mapRect (bounding_box ());
  QRect result_rect = state.rect ().intersected (transformed_rect.toRect ());
  if (result_rect.isNull ())
    return;

  QPixmap pixmap (result_rect.width (), result_rect.height ());
  pixmap.fill (Qt::transparent);
  QTransform pixmap_transform = QTransform::fromTranslate (-result_rect.x ()+ state.rect ().x (),-result_rect.y () +state.rect ().y ());

  QPainter group_painter (&pixmap);
  group_painter.setRenderHint (QPainter::Antialiasing);

  renderer_state new_state;
  new_state.set_rect (state.rect ());
  new_state.set_transform (item_transform * pixmap_transform);

  for (abstract_svg_item *child = m_svg_item->first_child (); child; child = child->next_sibling ())
    {
      const abstract_renderer_item *child_item = child->get_renderer_item ();
      if (child_item)
        child_item->draw (group_painter, new_state);
    }

  group_painter.end ();

  if (m_has_clip_path)
    {
      QPainterPath clip_path = new_state.transform ().map (m_clip_path);
      painter.setClipPath (clip_path);
    }

  painter.setTransform (QTransform ());
  painter.setOpacity (m_opacity);
  painter.drawPixmap (result_rect, pixmap, pixmap.rect ());
  return;
}

QRectF renderer_item_group::bounding_box () const 
{
  update_bbox ();
  return m_bbox;
}

void renderer_item_group::update_bbox () const
{
  if (m_bbox_calculated)
    return;

  QRectF new_box;
  for (const abstract_svg_item *child = m_svg_item->first_child (); child; child = child->next_sibling ())
    {
      const abstract_renderer_item *renderer_item = child->get_renderer_item ();
      if (!renderer_item)
        continue;

      new_box = new_box.united (renderer_item->bounding_box ());
    }

  m_bbox = transform ().mapRect (new_box);
  m_bbox_calculated = true;
}
