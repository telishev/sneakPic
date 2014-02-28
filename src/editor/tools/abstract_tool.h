#ifndef ABSTRACT_TOOL_H
#define ABSTRACT_TOOL_H

#include <QObject>

class mouse_event_t;
class QPainter;
class QRect;
class QTransform;
class overlay_renderer;
class actions_applier;

class svg_painter;
class mouse_shortcut_enum_union;

enum class gui_action_id;

class abstract_tool : public QObject
{
protected:
  svg_painter *m_painter;
  overlay_renderer        *m_overlay;
  actions_applier *m_actions_applier;
public:
  abstract_tool (svg_painter *painter);
  virtual ~abstract_tool ();

  void update ();
  void interrupt_action ();

  virtual void activate ();
  virtual void deactivate ();

  virtual void configure ();

  virtual void draw (QPainter &painter, const QRect &rect_to_draw, const QTransform &transform);

  virtual bool mouse_event (const mouse_event_t &m_event, mouse_shortcut_enum_union action);
  virtual bool action_triggered (gui_action_id id);
};

#endif // ABSTRACT_TOOL_H
