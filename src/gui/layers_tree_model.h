#ifndef LAYERS_TREE_MODEL_H
#define LAYERS_TREE_MODEL_H

#include <QAbstractItemModel>
#include <QIcon>

class layers_handler;

struct layers_tree_node;


class layers_tree_model : public QAbstractItemModel
{
  Q_OBJECT

  layers_handler *m_layers_handler;
  QIcon eye_closed_icon, eye_open_icon, layer_icon, folder_icon;
public:
  layers_tree_model ();
  virtual int rowCount (const QModelIndex &parent = QModelIndex()) const override;
  virtual int columnCount( const QModelIndex &parent = QModelIndex( ) ) const override;

  virtual QVariant data (const QModelIndex &index, int role = Qt::DisplayRole) const override;
  virtual QModelIndex parent (const QModelIndex &index) const override;
  virtual QModelIndex index (int row, int column, const QModelIndex &parent = QModelIndex()) const override;
  layers_handler *handler () { return m_layers_handler; }
  void index_clicked (const QModelIndex & index) const;

public slots:
  void update_model ();
  QModelIndex index_for_node (layers_tree_node *node) const;
  void set_layers_handler (layers_handler * handler);
  virtual bool setData( const QModelIndex &index, const QVariant &value, int role = Qt::EditRole );
  virtual Qt::ItemFlags flags( const QModelIndex &index ) const;
  virtual Qt::DropActions supportedDropActions() const;
  virtual Qt::DropActions supportedDragActions() const;

  virtual bool dropMimeData( const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent );
  virtual QMimeData *mimeData( const QModelIndexList &indexes ) const;
  virtual bool canDropMimeData( const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent ) const;
  virtual QStringList mimeTypes() const;

signals:
  void resize_needed ();
};

#endif // LAYERS_TREE_MODEL_H
