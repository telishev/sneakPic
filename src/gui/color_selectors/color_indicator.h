#ifndef COLOR_INDICATOR_H
#define COLOR_INDICATOR_H

#include <QColor>
#include <QWidget>

#include "gui/color_selectors/color_selector.h"
#include "editor/item_paint_server.h"


class color_indicator : public QWidget
{
  Q_OBJECT

  item_paint_server m_server;
  bool m_is_stroke;
public:
  color_indicator (bool is_stroke, QWidget *parent);
  ~color_indicator ();

  virtual QSize sizeHint () const;
  void set_paint_server (const item_paint_server &server);
  item_paint_server paint_server () const { return m_server; }

protected:
  virtual void paintEvent (QPaintEvent *event) override;
  virtual void mouseReleaseEvent (QMouseEvent *event) override;


signals:
  void clicked ();

private:
  QRect draw_rect () const;
};

#endif // COLOR_INDICATOR_H

