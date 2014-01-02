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

public:
  stroke_style ();
  ~stroke_style ();
  void init (settings_t *settings_arg);
  void init (items_selection *selection);
  void update_color_from_selection ();
  void apply_color_to_selection ();
  QColor *color () { return m_color; }

private:
  void set_selection_color ();
};

#endif // STROKE_STYLE_H
