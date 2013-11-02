#ifndef PATH_HANDLES_H
#define PATH_HANDLES_H

#include "renderer/overlay_items_container.h"

class items_selection;

class path_handles : public overlay_items_container
{
  const items_selection *m_selection;
public:
  path_handles (const items_selection *selection, overlay_renderer *overlay, svg_items_container *container);
  ~path_handles ();

  void update ();

protected:
  virtual renderable_item *create_overlay_item (const std::string &object) const override;
};

#endif // PATH_HANDLES_H
