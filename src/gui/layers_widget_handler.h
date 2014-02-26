#ifndef LAYERS_WIDGET_HANDLER
#define LAYERS_WIDGET_HANDLER

#include <QObject>

class dock_widget_builder;
class layers_handler;
class layers_tree_model;

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
  QToolButton *m_add_layer_btn, *m_remove_layer_btn;
  unique_ptr<connection> change_active_layer_connection;

public:
  layers_widget_handler (dock_widget_builder *dock_widget_builder_arg);
  ~layers_widget_handler ();

  void set_layers_handler (layers_handler *handler);

private slots:
  void update_active_layer ();
  void change_active_layer ();
  void index_clicked (const QModelIndex &index);
};

#endif //LAYERS_WIDGET_HANDLER

