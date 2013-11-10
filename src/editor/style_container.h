#ifndef STYLE_CONTAINER_H
#define STYLE_CONTAINER_H

class fill_style;

class style_container
{
  fill_style *m_fill_style;
public:
  style_container ();
  fill_style *get_fill_style () { return m_fill_style; }
};

#endif // STYLE_CONTAINER_H
