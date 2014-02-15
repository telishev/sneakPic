#ifndef PATH_ANCHOR_HANDLE_H
#define PATH_ANCHOR_HANDLE_H

#include "editor/base_anchor_handle.h"

#include <QPointF>
#include <string>

class QRect;
class QColor;

class svg_item_path;
class svg_path_geom;
class path_handles_editor;
class path_edit_operation;
class SkCanvas;
class SkPaint;
class SkRect;

enum class node_type_t : char;

struct single_path_point;

class path_anchor_handle : public base_anchor_handle
{
  path_handles_editor *m_editor;

  QPointF m_drag_start;
  QPointF m_drag_cur;

  unique_ptr<path_edit_operation> m_edit_operation;

public:
  path_anchor_handle (path_handles_editor *editor, svg_item_path *item, svg_path_geom_iterator path_it);
  virtual ~path_anchor_handle ();

  int point_id () const { return (int)m_path_it.point_index (); }
  std::string item_name () const;

  virtual QPointF get_handle_center () const override;

protected:
  virtual void set_mouse_hovered (bool hovered) override;
  virtual bool start_drag (QPointF local_pos) override;
  virtual bool drag (QPointF local_pos) override;
  virtual bool end_drag (QPointF local_pos) override;
  virtual node_type_t node_type () const override;
  virtual QTransform get_path_transform () const;

private:
  void apply_drag ();
  void move_point ();
  const svg_path_geom *get_path () const;
};

#endif // PATH_ANCHOR_HANDLE_H
