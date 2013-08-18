#include "svg/changed_items_container.h"

#include "common/debug_utils.h"

#include "renderer/event_items_changed.h"

#include "svg/items/svg_items_container.h"
#include "svg/items/svg_graphics_item.h"
#include "svg/items/svg_item_svg.h"


#include "svg/svg_document.h"



changed_items_container::changed_items_container (svg_document *document)
{
  m_document = document;
}

changed_items_container::~changed_items_container ()
{

}

void changed_items_container::child_added (const std::string &parent, const std::string &child_name, int /*insert_pos*/)
{
  m_changed_items.insert (child_name);
  m_layout_changed_items.insert (parent);
}

void changed_items_container::child_removed (const std::string &parent, const std::string &child_name, int /*pos*/)
{
  m_removed_items.insert (child_name);
  m_layout_changed_items.insert (parent);
}

void changed_items_container::child_moved (const std::string &parent, const std::string &/*child_name*/, int /*old_pos*/, int /*new_pos*/)
{
  m_layout_changed_items.insert (parent);
}

void changed_items_container::attribute_change_start (const std::string &parent, const abstract_attribute * /*computed_attribute*/)
{
  ///TODO: change children too
  m_changed_items.insert (parent);
}

void changed_items_container::attribute_change_end (const std::string &/*parent*/, const abstract_attribute * /*computed_attribute*/)
{
}

void changed_items_container::item_removed (const std::string &/*parent*/)
{
  /// handled in child_removed
}


abstract_state_t *changed_items_container::create_state ()
{
  DEBUG_PAUSE ("Not really undoable");
  return nullptr;
}

void changed_items_container::load_from_state (const abstract_state_t * /*state*/)
{
  DEBUG_PAUSE ("Not really undoable");
}

event_items_changed *changed_items_container::create_changed_items_event ()
{
  event_items_changed *ev = new event_items_changed;
  for (const std::string &item_name : m_changed_items)
    invalidate_bbox (item_name);

  for (const std::string &item_name : m_layout_changed_items)
    invalidate_bbox (item_name);

  m_document->root_svg_item ()->update_bbox ();

  for (const std::string &item_name : m_changed_items)
    {
      svg_graphics_item *graphics_item = get_graphics_item (item_name);
      if (!graphics_item)
        continue;

      ev->add_renderer_item (graphics_item->create_renderer_item ());
    }

  for (const std::string &item_name : m_layout_changed_items)
    {
      if (m_changed_items.find (item_name) != m_changed_items.end ())
        continue;

      svg_graphics_item *graphics_item = get_graphics_item (item_name);
      if (!graphics_item)
        continue;

      ev->add_layout_changed_item (graphics_item->create_renderer_item ());
    }

  for (const std::string &item_name : m_removed_items)
    {
      ev->add_removed_item (item_name);
    }
  clear ();
  return ev;
}

void changed_items_container::clear ()
{
  m_changed_items.clear ();
  m_layout_changed_items.clear ();
  m_removed_items.clear ();
}

void changed_items_container::invalidate_bbox (const std::string &item_name)
{
  svg_graphics_item *graphics_item = get_graphics_item (item_name);
  if (!graphics_item)
    return;

  graphics_item->invalidate_bbox ();
}

svg_graphics_item *changed_items_container::get_graphics_item (const std::string &item_name) const
{
  svg_items_container *container = m_document->item_container ();
  abstract_svg_item *item = container->get_item (item_name);
  return item ? item->to_graphics_item () : nullptr;
}

void changed_items_container::set_item_changed (const std::string &item)
{
  m_changed_items.insert (item);
}

void changed_items_container::set_item_layout_changed (const std::string &item)
{
  m_layout_changed_items.insert (item);
}

void changed_items_container::set_item_removed (const std::string &item)
{
  m_removed_items.insert (item);
}
