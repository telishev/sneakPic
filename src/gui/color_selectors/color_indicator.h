#ifndef COLOR_INDICATOR_H
#define COLOR_INDICATOR_H

#include <QColor>
#include <QWidget>

#include "gui/color_selectors/color_selector.h"

class QPainter;
class renderer_paint_server;

class color_indicator : public QWidget
{
  Q_OBJECT

  unique_ptr<renderer_paint_server> m_server;
  bool m_is_stroke;
public:
  color_indicator (bool is_stroke, QWidget *parent);
  ~color_indicator ();

  virtual QSize sizeHint () const;
  void set_paint_server (const renderer_paint_server *server);
  renderer_paint_server *paint_server () const { return m_server.get (); }

protected:
  virtual void paintEvent (QPaintEvent *event) override;
  virtual void mouseReleaseEvent (QMouseEvent *event) override;


signals:
  void clicked ();

private:
  QRect get_draw_rect () const;
  void draw_rect (QPainter &painter);
};

#endif // COLOR_INDICATOR_H

