#include "gui/stroke_config_widget.h"

#include <QHBoxLayout>
#include <QComboBox>
#include <QLabel>
#include <QDoubleSpinBox>

#include "utils/qt_utils.h"

#include "gui/connection.h"
#include "gui/gui_widget_view.h"
#include "gui/gui_model.h"

Q_DECLARE_METATYPE (Qt::PenJoinStyle);
Q_DECLARE_METATYPE (Qt::PenCapStyle);

stroke_config_widget::stroke_config_widget (gui_model *model, QWidget *parent)
  : QWidget (parent)
{
  m_view.reset (new gui_widget_view (model));
  setObjectName ("Stroke");
  m_stroke_style_layout = qt_utils::create_common_vbox_layout (this);
  init_linejoin_controller ();
  init_miterlimit_controller ();
  init_stroke_width_controller ();
  init_linecap_controller ();
  m_stroke_style_layout->addStretch ();
}

stroke_config_widget::~stroke_config_widget ()
{

}

void stroke_config_widget::init_linejoin_controller ()
{
  {
    QHBoxLayout *layout = qt_utils::create_intermediate_hbox_layout (m_stroke_style_layout);
    layout->addWidget (new QLabel ("Join:"));
    layout->addWidget (m_stroke_linejoin_combobox = new QComboBox (layout->parentWidget ()));
    layout->addStretch ();
    m_stroke_linejoin_combobox->addItem (QIcon (":/linejoin_miter.png"), "Miter", QVariant::fromValue <Qt::PenJoinStyle> (Qt::PenJoinStyle::SvgMiterJoin));
    m_stroke_linejoin_combobox->addItem (QIcon (":/linejoin_round.png"), "Round", QVariant::fromValue <Qt::PenJoinStyle> (Qt::PenJoinStyle::RoundJoin));
    m_stroke_linejoin_combobox->addItem (QIcon (":/linejoin_bevel.png"), "Bevel", QVariant::fromValue <Qt::PenJoinStyle> (Qt::PenJoinStyle::BevelJoin));
    m_view->add_widget (gui_model_role_t::LINEJOIN, m_stroke_linejoin_combobox);
  }
}

void stroke_config_widget::init_miterlimit_controller ()
{
  {
    QHBoxLayout *layout = qt_utils::create_intermediate_hbox_layout (m_stroke_style_layout);
    layout->addWidget (new QLabel ("Miter limit:"));
    layout->addWidget (m_stroke_miterlimit_spinbox = qt_utils::create_double_spinbox (layout->parentWidget (), 100.0, 2));
    m_view->add_widget (gui_model_role_t::STROKE_MITER, m_stroke_miterlimit_spinbox);
    layout->addStretch ();
  }
}

void stroke_config_widget::init_stroke_width_controller ()
{
  {
    QHBoxLayout *layout = qt_utils::create_intermediate_hbox_layout (m_stroke_style_layout);
    layout->addWidget (new QLabel ("Width:"));
    layout->addWidget (m_stroke_width_spinbox = qt_utils::create_double_spinbox (layout->parentWidget (), 1000.0));
    m_view->add_widget (gui_model_role_t::STROKE_WIDTH, m_stroke_width_spinbox);
    layout->addStretch ();
  }
}

void stroke_config_widget::init_linecap_controller ()
{
  {
    QHBoxLayout *layout = qt_utils::create_intermediate_hbox_layout (m_stroke_style_layout);
    layout->addWidget (new QLabel ("Cap:"));
    layout->addWidget (m_stroke_linecap_combobox = new QComboBox (layout->parentWidget ()));
    layout->addStretch ();
    m_stroke_linecap_combobox->addItem (QIcon (":/linecap_butt.png"), "Butt", QVariant::fromValue <Qt::PenCapStyle> (Qt::PenCapStyle::FlatCap));
    m_stroke_linecap_combobox->addItem (QIcon (":/linecap_round.png"), "Round", QVariant::fromValue <Qt::PenCapStyle> (Qt::PenCapStyle::RoundCap));
    m_stroke_linecap_combobox->addItem (QIcon (":/linecap_cap.png"), "Cap", QVariant::fromValue <Qt::PenCapStyle> (Qt::PenCapStyle::SquareCap));
    m_view->add_widget (gui_model_role_t::LINECAP, m_stroke_linecap_combobox);
  }
}
