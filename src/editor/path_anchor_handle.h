#ifndef PATH_ANCHOR_HANDLE_H
#define PATH_ANCHOR_HANDLE_H

#include "editor/abstract_handle.h"
#include "path/svg_path.h"
#include <QPointF>
#include <string>

class QRect;
class QColor;

class svg_item_path;
class svg_path;
class path_handles_editor;
class path_edit_operation;
class SkCanvas;
class SkPaint;
class SkRect;

enum class node_type_t : char;

struct single_path_point;

class path_anchor_handle : public abstract_handle
{
  svg_item_path *m_item;
  path_handles_editor *m_editor;
  svg_path_iterator m_path_it;

  QPointF m_drag_start;
  QPointF m_drag_cur;
  bool m_is_highlighted;

  std::unique_ptr<path_edit_operation> m_edit_operation;

public:
  path_anchor_handle (path_handles_editor *editor, svg_item_path *item, svg_path_iterator path_it);
  virtual ~path_anchor_handle ();

  int point_id () const { return (int)m_path_it.point_index (); }
  std::string item_name () const;

  QPointF get_handle_center () const;

protected:
  virtual int distance_to_mouse (QPoint screen_pos, QTransform transform) const override;
  virtual void set_mouse_hovered (bool hovered) override;
  virtual bool start_drag (QPointF local_pos) override;
  virtual bool drag (QPointF local_pos) override;
  virtual bool end_drag (QPointF local_pos) override;
  virtual void draw (SkCanvas &canvas, const renderer_state &state, const renderer_config *config) const override;

private:
  QRect get_element_rect (QTransform transform) const;

  void apply_drag ();
  void move_point ();
  const svg_path *get_path () const;
  node_type_t node_type () const;
};

#endif // PATH_ANCHOR_HANDLE_H
