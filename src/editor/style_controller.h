#ifndef STYLE_CONTROLLER_H
#define STYLE_CONTROLLER_H

#include <QObject>

#include "item_paint_style.h"
#include "gui\gui_model.h"

class settings_t;
class items_selection;
class item_paint_style;
class svg_painter;
class item_paint_server;

enum class style_controller_role_t
{
  /// stroke_config
  STROKE_WIDTH, /// double
  STROKE_MITER, /// double
  LINECAP, /// Qt::PenCapStyle
  LINEJOIN, // Qt::PenJoinStyle

  /// color

  FILL_COLOR, /// item_paint_server
  STROKE_COLOR, /// item_paint_server
  CURRENT_COLOR, /// QColor
  CURRENT_COLOR_TEMP, /// QColor

  IS_SELECTED_FILL, /// bool
};

enum class selected_style
{
  EDITOR_STYLE = 0,
  SELECTED_STYLE,

  COUNT
};

class style_controller : public gui_model<style_controller_role_t>
{
  Q_OBJECT

  selected_style m_current_style;
  item_paint_style m_styles[(int) selected_style::COUNT];
  svg_painter *m_painter;
  bool m_is_selected_fill;

public:
  style_controller (settings_t *settings_arg);
  const item_paint_style *active_style () const;
  void set_painter (svg_painter *painter);

  selected_style current_style () { return m_current_style; }
  void set_current_style (selected_style style) { m_current_style = style; }
  const item_paint_server *active_server () const;

  bool is_selected_fill () const { return m_is_selected_fill; }

  virtual QVariant data (style_controller_role_t role) const override;
  virtual void set_model_data (const std::map<style_controller_role_t, QVariant> &data_map) override;

private slots:
  void selection_or_items_changed ();
  void update_from_color_picker (const QColor &color);

private:
  item_paint_server *active_server ();
  void send_items_changed ();
  std::set<style_controller_role_t> all_items_set () const;

};

#endif // STYLE_CONTROLLER_H
