#include "fill_stroke_widget.h"

#include "color_selectors/color_indicator.h"

#include "editor/item_paint_style.h"

#include "gui/gui_widget_view.h"
#include "gui_model.h"
#include "connection.h"

Q_DECLARE_METATYPE (item_paint_server);

class color_indicator_gui_widget : public gui_widget
{
  color_indicator *m_indicator;
public:
  color_indicator_gui_widget (color_indicator *indicator) : m_indicator (indicator) {}
  virtual void set_value (QVariant value) override { m_indicator->set_paint_server (value.value<item_paint_server> ());}
  virtual QVariant value () const override { return QVariant::fromValue (m_indicator->paint_server ()); }
};

class fill_stroke_gui_widget : public gui_widget
{
  fill_stroke_widget *m_fill_stroke;
public:
  fill_stroke_gui_widget (fill_stroke_widget *fill_stroke, color_indicator *fill, color_indicator *stroke) : m_fill_stroke (fill_stroke)
  {
    CONNECT (fill, &color_indicator::clicked, [=] () {fill_stroke->set_is_selected_fill (true); emit data_changed (); });
    CONNECT (stroke, &color_indicator::clicked, [=] () {fill_stroke->set_is_selected_fill (false); emit data_changed (); });
  }

  virtual void set_value (QVariant value) override { m_fill_stroke->set_is_selected_fill (value.toBool ()); }
  virtual QVariant value () const override { return m_fill_stroke->is_selected_fill (); }
};

fill_stroke_widget::fill_stroke_widget (gui_model *model, QWidget *parent)
  : QWidget (parent)
{
  m_view.reset (new gui_widget_view (model));
  m_fill = new color_indicator (false, this);
  m_stroke = new color_indicator (true, this);

  m_view->add_gui_widget (gui_model_role_t::FILL_COLOR, new color_indicator_gui_widget (m_fill));
  m_view->add_gui_widget (gui_model_role_t::STROKE_COLOR, new color_indicator_gui_widget (m_stroke));
  m_view->add_gui_widget (gui_model_role_t::IS_SELECTED_FILL, new fill_stroke_gui_widget (this, m_fill, m_stroke));

  QSize size = m_fill->sizeHint ();
  m_fill->setGeometry (QRect (0, 0, size.width (), size.height ()));
  m_stroke->setGeometry (QRect (0, 0, size.width (), size.height ()));
  m_stroke->move (stroke_pos ());

  set_is_selected_fill (model->data (gui_model_role_t::IS_SELECTED_FILL).toBool ());
}

fill_stroke_widget::~fill_stroke_widget ()
{

}

QSize fill_stroke_widget::sizeHint () const 
{
  return m_stroke->sizeHint () + QSize (stroke_pos ().x () + 1, stroke_pos ().y () + 1);
}

QPoint fill_stroke_widget::stroke_pos () const
{
  QSize size = m_fill->sizeHint ();
  return QPoint (size.width () / 2, size.height () / 2);
}

void fill_stroke_widget::set_is_selected_fill (bool selected)
{
  m_is_selected_fill = selected;
  if (m_is_selected_fill)
    m_fill->raise ();
  else
    m_stroke->raise ();
}



