#ifndef STYLE_CONTAINER_H
#define STYLE_CONTAINER_H

class fill_style;
class items_selection;
class settings_t;
class stroke_style;

class style_container
{
  fill_style *m_fill_style;
  stroke_style *m_stroke_style;
public:
  style_container ();
  fill_style *get_fill_style () { return m_fill_style; }
  stroke_style *get_stroke_style () { return m_stroke_style; }
  void init (settings_t *settings);
  void init (items_selection *selection);
};

#endif // STYLE_CONTAINER_H
