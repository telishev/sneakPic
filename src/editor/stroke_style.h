#ifndef STROKE_STYLE_H
#define STROKE_STYLE_H

class QColor;

class items_selection;
class settings_t;

namespace Qt
{
  enum PenJoinStyle;
  enum PenCapStyle;
};

class stroke_style
{
  QColor *m_color;
  QColor *m_internal_color;
  items_selection *m_selection;
  double m_width;
  settings_t *m_settings;
  Qt::PenJoinStyle m_linejoin;
  Qt::PenCapStyle m_linecap;
  double m_miterlimit;
public:
  stroke_style ();
  ~stroke_style ();
  void init (settings_t *settings_arg);
  void init (items_selection *selection);
  void update_from_selection ();
  void apply_color_to_selection ();
  void update_width (double value);
  void update_miterlimit (double value);
  QColor *color () { return m_color; }
  double stroke_width () const { return m_width; }
  double stroke_miterlimit () const { return m_miterlimit; }
  Qt::PenJoinStyle stroke_linejoin () const;
  Qt::PenCapStyle stroke_linecap () const;
  void update_linejoin( Qt::PenJoinStyle value);
   void update_linecap (Qt::PenCapStyle value);

private:
  void set_selection_color ();
  bool is_single_item_selected ();
};

#endif // STROKE_STYLE_H
