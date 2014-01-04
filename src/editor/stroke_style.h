#ifndef STROKE_STYLE_H
#define STROKE_STYLE_H

class QColor;

class items_selection;
class settings_t;

class stroke_style
{
  QColor *m_color;
  QColor *m_internal_color;
  items_selection *m_selection;
  double m_stroke_width;
  settings_t *m_settings;

public:
  stroke_style ();
  ~stroke_style ();
  void init (settings_t *settings_arg);
  void init (items_selection *selection);
  void update_from_selection ();
  void apply_color_to_selection ();
  void update_line_width (double value);
  QColor *color () { return m_color; }
  double stroke_width () { return m_stroke_width; };

private:
  void set_selection_color ();
  bool is_single_item_selected ();
};

#endif // STROKE_STYLE_H
