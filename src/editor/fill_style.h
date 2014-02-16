#ifndef FILL_STYLE_H
#define FILL_STYLE_H

#include <memory>

class QColor;

class settings_t;
class items_selection;

class fill_style
{
  QColor *m_color;
  QColor *m_internal_color;
  items_selection *m_selection;
public:
  fill_style ();
  ~fill_style ();
  void init (settings_t *settings_arg);
  void init (items_selection *selection);
  QColor *color () { return m_color; }
  void apply_color_to_selection ();
  void update_from_selection ();
  void set_color( QColor color );
private:
  void set_selection_color ();
};

#endif // FILL_STYLE_H
