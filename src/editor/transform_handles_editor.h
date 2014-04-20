#ifndef TRANSFORM_HANDLES_EDITOR_H
#define TRANSFORM_HANDLES_EDITOR_H

#include "editor/abstract_handle.h"
#include "editor/base_handle.h"
#include "editor/handles_editor.h"

class QRectF;
class QTransform;
class transform_handles_editor;
class renderer_overlay_path;

enum stretch_type
{
  LEFT,
  RIGHT,
  TOP,
  BOTTOM,
  TOPLEFT,
  TOPRIGHT,
  BOTTOMLEFT,
  BOTTOMRIGHT,

  COUNT,
};

class transform_handle : public base_handle
{
  stretch_type m_type = LEFT;
  QRectF m_bbox;
  QPointF m_origin_pos;
  bool m_drag_started = false;
  QPointF m_cur_pos;
  transform_handles_editor &m_editor;

protected:
  virtual QPointF get_handle_center () const override;
  virtual bool is_visible () const override;

public:
  transform_handle (stretch_type type, const QRectF &bbox, transform_handles_editor &editor);
  ~transform_handle ();

  virtual handle_type get_handle_type () const;
  virtual float handle_rotation () const;
  virtual int get_handle_size () const;

  virtual bool start_drag (QPointF local_pos, QTransform transform) override;
  virtual bool drag (QPointF local_pos, QTransform transform) override;
  virtual bool end_drag (QPointF local_pos, QTransform transform) override;

  virtual void interrupt_drag () override;

  };

class transform_handles_editor : public handles_editor
{
  unique_ptr<QRectF> m_bbox;
  vector<unique_ptr<transform_handle>> m_handles;
  unique_ptr<renderer_overlay_path> m_contour_renderer;
  unique_ptr<QTransform> m_transform;
  bool m_drag_started = false;

public:
  transform_handles_editor (overlay_renderer *overlay, svg_painter *painter, actions_applier *applier);
  ~transform_handles_editor ();
  void update_transform (const QTransform &new_rect);
  void finalize_transform ();
  bool drag_started () { return m_drag_started; }
  void set_drag_started (bool value);

protected:
  virtual vector<abstract_handle *> get_handles () const;
  virtual void update_handles_impl ();
private:
};

#endif // TRANSFORM_HANDLES_EDITOR_H

