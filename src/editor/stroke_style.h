#ifndef STROKE_STYLE_H
#define STROKE_STYLE_H

class QColor;

class items_selection;
class settings_t;

namespace Qt
{
  enum PenJoinStyle : int;
};

class stroke_style
{
  QColor *m_color;
  QColor *m_internal_color;
  items_selection *m_selection;
  double m_stroke_width;
  settings_t *m_settings;
  Qt::PenJoinStyle m_linejoin;
public:
  stroke_style ();
  ~stroke_style ();
  void init (settings_t *settings_arg);
  void init (items_selection *selection);
  void update_from_selection ();
  void apply_color_to_selection ();
  void update_line_width (double value);
  QColor *color () { return m_color; }
  double stroke_width () const { return m_stroke_width; };
  Qt::PenJoinStyle stroke_linejoin () const;
  void update_linejoin( Qt::PenJoinStyle value );

private:
  void set_selection_color ();
  bool is_single_item_selected ();

};

#endif // STROKE_STYLE_H
