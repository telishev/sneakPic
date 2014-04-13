#ifndef SELECTION_HANDLES_EDITOR_H
#define SELECTION_HANDLES_EDITOR_H

class svg_painter;
class actions_applier;
class overlay_renderer;
class handles_renderer;
class element_handles;
class abstract_svg_item;
class abstract_handle;

enum class handle_priority;

class QPointF;
class QPoint;

#include <map>
#include <string>
#include <memory>
#include "handles_editor.h"


class selection_handles_editor : public handles_editor
{
private:
  typedef map<string, unique_ptr<element_handles>> map_t;
  map_t m_handles;

public:
  selection_handles_editor (overlay_renderer *overlay, svg_painter *painter, actions_applier *applier);
  virtual ~selection_handles_editor ();
    map_t::const_iterator begin () const { return m_handles.begin (); }
  map_t::const_iterator end () const { return m_handles.end (); }
  element_handles *handles_for_item (const string &item) const;

  void set_handle_for_item (const char *name, element_handles *handles);

protected:
  virtual element_handles *create_handles_for_item (abstract_svg_item *item) = 0;
  virtual void update_handles_impl () override;
  virtual std::vector<abstract_handle *> get_handles () const override;

private:
  void add_item (abstract_svg_item *item);
  void remove_item (const string &item);
  void clear_items ();
};

#endif // SELECTION_HANDLES_EDITOR_H
