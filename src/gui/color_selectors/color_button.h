#ifndef COLOR_BUTTON_H
#define COLOR_BUTTON_H

#include <QToolButton>

class renderer_paint_server;

class color_button : public QToolButton
{
public:
  color_button (QWidget *parent = 0);
  ~color_button ();

  void set_paint_server (const renderer_paint_server *server);
private:
  QIcon create_icon (const renderer_paint_server *server);

};

#endif // COLOR_BUTTON_H
