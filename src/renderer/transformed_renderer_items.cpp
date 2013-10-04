#include "renderer/transformed_renderer_items.h"

#include "renderer/renderer_state.h"


transformed_renderer_items::transformed_renderer_items ()
  : abstract_renderer_item ("#current_transforming_item")
{
}

transformed_renderer_items::~transformed_renderer_items ()
{

}

void transformed_renderer_items::draw (SkCanvas &canvas, const renderer_state &state, const renderer_config *config) const 
{
  if (m_base_items.empty ())
    return;

  renderer_state new_state (state.rect (), m_transform *state.transform ());
  for (auto &item : m_base_items)
    item->draw (canvas, new_state, config);
}

void transformed_renderer_items::update_bbox_impl ()
{
}

QRectF transformed_renderer_items::bounding_box_impl () const 
{
  if (m_base_items.empty ())
    return QRectF ();

  QRectF bbox = m_base_items[0]->bounding_box ();
  for (auto &item : m_base_items)
    bbox = bbox.united (item->bounding_box ());


  return m_transform.mapRect (bbox);
}

void transformed_renderer_items::set_base_item_transform (const QTransform &transform)
{
  m_transform = transform;
}

void transformed_renderer_items::add_base_item (abstract_renderer_item *base_item)
{
  m_base_items.push_back (std::unique_ptr<abstract_renderer_item> (base_item));
}

void transformed_renderer_items::clear_base_items ()
{
  m_base_items.clear ();
}
