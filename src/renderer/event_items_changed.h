#ifndef EVENT_ITEMS_CHANGED_H
#define EVENT_ITEMS_CHANGED_H

#include "renderer/abstract_renderer_event.h"

#include <string>
#include <vector>

class abstract_renderer_item;

class event_items_changed : public abstract_renderer_event
{
  std::vector<abstract_renderer_item *> m_changed_items;
  std::vector<abstract_renderer_item *> m_layout_changed_items;
  std::vector<std::string> m_removed_items;
public:
  event_items_changed ();
  ~event_items_changed ();

  void add_renderer_item (abstract_renderer_item *item);
  void add_layout_changed_item (abstract_renderer_item *item);
  void add_removed_item (const std::string &item);

  virtual void process (renderer_thread *renderer) override;
  virtual bool interrupt_rendering () const override { return true; }

private:
  void change_item (renderer_thread *renderer, abstract_renderer_item *item, bool invalidate_prev_bbox);
};

#endif // EVENT_ITEMS_CHANGED_H
