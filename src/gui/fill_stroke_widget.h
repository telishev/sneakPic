#ifndef FILL_STROKE_WIDGET_H
#define FILL_STROKE_WIDGET_H

#include <QWidget>
#include <QColor>
#include "editor/item_paint_server.h"
#include "editor/item_paint_style.h"

class color_indicator;
class item_paint_server;
class item_paint_style;
class gui_widget_view;
class gui_model;

class fill_stroke_widget : public QWidget
{
  Q_OBJECT

  color_indicator *m_fill;
  color_indicator *m_stroke;
  bool m_is_selected_fill;

  unique_ptr<gui_widget_view> m_view;
  gui_model *m_model;

public:
  fill_stroke_widget (gui_model *model, QWidget *parent = 0);
  ~fill_stroke_widget ();

  bool is_selected_fill () const { return m_is_selected_fill; }
  void set_is_selected_fill (bool selected);
protected:
  QSize sizeHint () const override;

private slots:
  void swap_fill_stroke ();

private:
  QPoint stroke_pos () const;

};

#endif // FILL_STROKE_WIDGET_H

