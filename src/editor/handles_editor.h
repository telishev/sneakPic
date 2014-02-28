#ifndef HANDLES_EDITOR_H
#define HANDLES_EDITOR_H

class svg_painter;
class actions_applier;
class overlay_renderer;
class handles_renderer;
class element_handles;
class abstract_svg_item;
class abstract_handle;

class QPointF;
class QPoint;

#include <map>
#include <string>
#include <memory>


class handles_editor
{
protected:
  svg_painter *m_painter;
  actions_applier *m_applier;
  overlay_renderer *m_overlay;

private:
  handles_renderer *m_renderer;
  abstract_handle *m_cur_handle;
  abstract_handle *m_highlighted_handle;

  typedef map<string, unique_ptr<element_handles>> map_t;
  map_t m_handles;

public:
  handles_editor (overlay_renderer *overlay, svg_painter *painter, actions_applier *applier);
  virtual ~handles_editor ();

  void update_handles ();

  map_t::const_iterator begin () const { return m_handles.begin (); }
  map_t::const_iterator end () const { return m_handles.end (); }
  element_handles *handles_for_item (const string &item) const;

  void set_handle_for_item (const char *name, element_handles *handles);

protected:
  virtual element_handles *create_handles_for_item (abstract_svg_item *item) = 0;
  virtual void update_handles_impl () {}

  void interrupt ();

  abstract_handle *get_handle_by_pos (QPointF screen_pos) const;

private:
  void add_item (abstract_svg_item *item);
  void remove_item (const string &item);
  void clear_items ();

  bool start_drag (QPointF pos);
  bool drag_handle (QPointF pos);
  bool end_drag (QPointF pos);

  bool interrupt_all ();

  bool highlight_handle (QPointF pos);

  QPointF get_local_pos (QPointF screen_pos) const;
  abstract_handle *get_handle_by_element (QPoint screen_pos, element_handles *element, double &distance) const;
};

#endif // HANDLES_EDITOR_H
