#include "gui/layers_tree_model.h"
#include "svg/layers_handler.h"
#include "gui/connection.h"

#include <QIcon>
#include <QMimeData>

#include "common/common_utils.h"

QVariant layers_tree_model::data (const QModelIndex &index, int role /*= Qt::DisplayRole*/) const
{
  switch (index.column ())
    {
    case 1:
      switch (role)
        {
        case Qt::DisplayRole:
        case Qt::EditRole:
          return m_layers_handler->get_layer_name (index.row ());
        }
      return QVariant ();
    case 0:
      switch (role)
      {
      case Qt::DisplayRole:
        return "";
      case Qt::DecorationRole:
        return m_layers_handler->is_layer_visible (index.row ()) ? eye_open_icon : eye_closed_icon;
      }
      return QVariant ();
    }
  return QVariant ();
}

layers_tree_model::layers_tree_model ()
{
  m_layers_handler = 0;
  eye_closed_icon = QIcon (":/eye_closed.png");
  eye_open_icon = QIcon (":/eye_open.png");
}

void layers_tree_model::update_model ()
{
  beginResetModel ();
  endResetModel ();
}

int layers_tree_model::rowCount (const QModelIndex &parent /*= QModelIndex()*/) const
{
  if (!m_layers_handler)
    return 0;

  if (parent == QModelIndex ())
    return (int) m_layers_handler->layers_count ();
  else
    return 0;
}

QModelIndex layers_tree_model::index (int row, int column, const QModelIndex &parent /*= QModelIndex()*/) const
{
  return hasIndex (row, column, parent) ? createIndex (row, column) : QModelIndex();
}

int layers_tree_model::columnCount (const QModelIndex &/*parent = QModelIndex()*/) const
{
  return 2;
}

QModelIndex layers_tree_model::index_for_layer (int layer_num) const
{
  return index (layer_num, 0);
}

void layers_tree_model::set_layers_handler (layers_handler *handler)
{
  m_layers_handler = handler;
  CONNECT (handler, &layers_handler::layers_changed, this, &layers_tree_model::update_model);
  update_model ();
}

bool layers_tree_model::setData (const QModelIndex &index, const QVariant &value, int role /*= Qt::EditRole */)
{
  switch (role)
    {
    case Qt::EditRole:
      switch (index.column ())
        {
        case 1:
          m_layers_handler->rename_layer (index.row (), value.toString ());
          return true;
        }
    }
  return false;
}

Qt::ItemFlags layers_tree_model::flags (const QModelIndex &index) const
{
  auto flags = QAbstractItemModel::flags (index);
  if (index == QModelIndex ())
    flags |= Qt::ItemIsDropEnabled;

  switch (index.column ())
  {
    case 1:
      flags |= (Qt::ItemIsEditable | Qt::ItemIsDragEnabled);
      break;
  }
  return flags;
}

Qt::DropActions layers_tree_model::supportedDropActions () const
{
   return Qt::MoveAction;
}

Qt::DropActions layers_tree_model::supportedDragActions () const
{
   return Qt::MoveAction;
}

const char *format = "internal";

bool layers_tree_model::dropMimeData (const QMimeData *data, Qt::DropAction /*action*/, int row, int /*column*/, const QModelIndex &/*parent*/)
{
  m_layers_handler->move_layer (data->data (format).toInt (), row);
  return true;
}

QMimeData *layers_tree_model::mimeData (const QModelIndexList &indexes) const
{
  // For our situation it's always 1 index, we need to pack row num
  QMimeData *data = new QMimeData ();
  data->setData (format, QByteArray::number (indexes[0].row ()));
  return data;
}

bool layers_tree_model::canDropMimeData (const QMimeData * /*data*/, Qt::DropAction /*action*/, int /*row*/, int /*column*/, const QModelIndex &/*parent*/) const
{
  return true;
}

QStringList layers_tree_model::mimeTypes () const
{
  QStringList sl;
  sl << format;
  return sl;
}
