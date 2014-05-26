#include "renderer/path_storage.h"

#include "svg/items/abstract_svg_item.h"
#include "svg/items/svg_item_path.h"
#include "svg/items/svg_items_container.h"

#include "svg/svg_document.h"

#include <QPainterPath>
#include <QTransform>
#include "common/debug_utils.h"
#include "svg/items/svg_item_type.h"

QString path_storage::path_to_svg (path_id idw) const
{
  switch (idw)
    {
    case path_id::DOUBLE_HEADED_ARROW: return ":/paths/double_headed_arrow.svg";
    case path_id::ROTATE_ARROW: return ":/paths/rotate_arrow.svg";
    case path_id::COUNT: return "";
    }
  return "";
}

unique_ptr<QPainterPath> path_storage::generate_path (const QString &path)
{
  svg_document doc;
  if (!doc.read_file (path))
    {
      DEBUG_PAUSE ("Svg path could not be loaded"); // Path included in path_storage not found (possibly path is wrong)
      return {};
    }
  svg_graphics_item *item = nullptr;
  for (auto &&item_it : *doc.item_container ())
    {
      item = item_it->to_graphics_item ();
      if (item != nullptr && item->type () == svg_item_type::PATH)
        break;
    }
  auto base_shap_item_ptr = dynamic_cast<svg_item_path *> (item);
  if (base_shap_item_ptr == nullptr)
    {
      DEBUG_PAUSE ("Path item is not found in svg");
      return {};
    }
  else
    return make_unique<QPainterPath> (base_shap_item_ptr->full_transform ().map (base_shap_item_ptr->get_path ()));
}

path_storage::path_storage ()
{
  for (int i = 0; i < (int) path_id::COUNT; i++)
    m_data.push_back (generate_path (path_to_svg (static_cast<path_id> (i))));
}

const path_storage &path_storage::ref ()
{
  static path_storage inst;
  return inst;
}

const QPainterPath & path_storage::path (path_id id) const
{
  return *m_data[(int) id];
}
