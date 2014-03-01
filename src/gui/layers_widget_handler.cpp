#include "svg/layers_handler.h"
#include "gui/layers_widget_handler.h"
#include "gui/layers_tree_model.h"
#include "gui/dock_widget_builder.h"

#include "gui/utils/qt_utils.h"

#include <QTreeWidget>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QSlider>
#include <QToolButton>

#include <stack>

#include "gui/connection.h"
#include "utils/override_undo_filter.h"

layers_widget_handler::layers_widget_handler (dock_widget_builder *dock_widget_builder_arg)
{
  m_dock_widget_builder = dock_widget_builder_arg;
  m_widget = new QWidget ();
  m_widget->setObjectName ("Layers");
  m_handler = 0;
  std::stack<QBoxLayout *> layouts;

  layouts.push (qt_utils::create_common_vbox_layout (m_widget));
  {
    layouts.top ()->addWidget (m_layers_view = new QTreeView ());
    m_layers_model.reset (new layers_tree_model ());
    m_layers_view->setSelectionMode (QAbstractItemView::SingleSelection);
    m_layers_view->setModel (m_layers_model.get ());
    m_layers_view->header ()->setVisible (false);
    layouts.push(qt_utils::create_intermediate_hbox_layout(layouts.top ()));
    {
      layouts.top()->addWidget(m_opacity_slider = new QSlider (Qt::Horizontal));
      m_opacity_slider->setRange(0, 100);
      m_opacity_slider->setTracking (false);
    }
    layouts.pop();

    layouts.push (qt_utils::create_intermediate_hbox_layout (layouts.top ()));
    {
      layouts.top ()->addWidget (m_add_layer_btn = new QToolButton ());
      m_add_layer_btn->setText ("+");
      layouts.top ()->addWidget (m_remove_layer_btn = new QToolButton ());
      m_remove_layer_btn->setText ("-");
      layouts.top ()->addStretch ();
      layouts.pop ();
    }
    layouts.pop ();
  }
  CONNECT (m_layers_view, &QTreeView::clicked, this, &layers_widget_handler::index_clicked);
  change_opacity_connection = CONNECT (m_opacity_slider, &QSlider::valueChanged, this, &layers_widget_handler::change_opacity);
  change_active_layer_connection = CONNECT (m_layers_view->selectionModel (), &QItemSelectionModel::currentChanged, this, &layers_widget_handler::change_active_layer);

  m_layers_view->setEditTriggers (QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);
  m_layers_view->setDragDropMode (QAbstractItemView::InternalMove);
  m_layers_view->setDropIndicatorShown (true);
  m_layers_view->installEventFilter (new override_undo_filter (m_layers_view));
  m_dock_widget_builder->add_widget (m_widget, Qt::RightDockWidgetArea, visibility_state::visible, Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
}

void layers_widget_handler::change_opacity ()
{
  m_handler->set_active_layer_opacity (m_opacity_slider->value ());
}

void layers_widget_handler::set_layers_handler (layers_handler *handler)
{
  m_handler = handler;
  m_layers_model->set_layers_handler (handler);
  CONNECT (m_handler, &layers_handler::layers_changed, this, &layers_widget_handler::update_active_layer);
  CONNECT (m_handler, &layers_handler::active_layer_changed, this, &layers_widget_handler::update_opacity_slider);
  CONNECT (m_add_layer_btn, &QToolButton::clicked, m_handler, &layers_handler::add_new_layer_slot);
  CONNECT (m_remove_layer_btn, &QToolButton::clicked, m_handler, &layers_handler::remove_active_layer);
  update_active_layer ();
  update_opacity_slider ();
}

void layers_widget_handler::change_active_layer ()
{
  m_handler->set_active_layer (m_layers_view->currentIndex ().row ());
}

layers_widget_handler::~layers_widget_handler()
{

}

void layers_widget_handler::index_clicked (const QModelIndex &index)
{
  switch (index.column ())
  {
  case 0:
    m_handler->toggle_layer_visibility (index.row ());
    m_layers_view->viewport ()->update ();
    break;
  }

}

void layers_widget_handler::update_active_layer ()
{
  TEMPORARY_DISCONNECT (change_active_layer_connection);
  m_layers_view->setCurrentIndex (m_layers_model->index_for_layer (m_handler->active_layer_index ()));
  m_layers_view->resizeColumnToContents (0);
}

void layers_widget_handler::update_opacity_slider ()
{
  TEMPORARY_DISCONNECT (change_opacity_connection);
  m_opacity_slider->setEnabled (m_handler->is_layer_selected ());
  m_opacity_slider->setSliderPosition (m_handler->get_active_layer_opacity ());
}
