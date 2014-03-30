#ifndef LAYERS_WIDGET_HANDLER
#define LAYERS_WIDGET_HANDLER

#include <QObject>

class dock_widget_builder;
class layers_handler;
class layers_tree_model;

class QSlider;
class QToolButton;
class QTreeWidget;
class QTreeView;

class connection;

class layers_widget_handler : public QObject
{
  Q_OBJECT

  dock_widget_builder *m_dock_widget_builder;
  QWidget *m_widget;
  layers_handler *m_handler;
  QTreeView *m_layers_view;
  unique_ptr<layers_tree_model> m_layers_model;
  QToolButton *m_add_layer_btn, *m_remove_layer_btn, *m_add_folder_btn;
  unique_ptr<connection> change_active_layer_connection, change_opacity_connection, index_collapsed_connection, index_expanded_connection;
  QSlider *m_opacity_slider;

public:
  layers_widget_handler (dock_widget_builder *dock_widget_builder_arg);
  ~layers_widget_handler ();

  void set_layers_handler (layers_handler *handler);
  void change_opacity ();

private:
  void update_expanded_state (QModelIndex &index);

private slots:
  void resize_to_contents ();
  void update_on_layers_change ();
  void change_active_layer ();
  void index_clicked (const QModelIndex &index);
  void index_expanded (const QModelIndex &index);
  void index_collapsed (const QModelIndex &index);
  void update_opacity_slider ();
  };

#endif //LAYERS_WIDGET_HANDLER


