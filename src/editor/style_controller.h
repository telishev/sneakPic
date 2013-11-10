#ifndef STYLE_CONTROLLER_H
#define STYLE_CONTROLLER_H

#include <QObject>

class settings_t;
class items_selection;
class style_container;
class svg_painter;

enum class selected_style
{
  EDITOR_STYLE = 0,
  SELECTED_STYLE,

  COUNT
};

class style_controller : public QObject
{
  Q_OBJECT

  selected_style m_current_style;
  style_container *m_containers[selected_style::COUNT];
  svg_painter *m_painter;
public:
  style_controller (settings_t *settings_arg);
  style_container *active_container ();
  void set_painter (svg_painter *painter);
  selected_style current_style () {return m_current_style; }
  void switch_to (selected_style current_style_arg );
public slots:
  void update_color_momentarily ();
  void apply_changes ();
private slots:
  void selection_changed ();
signals:
  void controller_updates_needed ();
};

#endif // STYLE_CONTROLLER_H