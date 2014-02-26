#include "gui/layers_tree_model.h"
#include "svg/layers_handler.h"
#include "gui/connection.h"

#include <QIcon>

QVariant layers_tree_model::data (const QModelIndex &index, int role /*= Qt::DisplayRole*/) const
{
  switch (index.column ())
    {
    case 1:
      switch (role)
        {
        case Qt::DisplayRole:
        case Qt::EditRole:
          return index.row () < m_layers_handler->layers_count ()  ? m_layers_handler->get_layer_name (index.row ()) : "(No Layer)";
        }
      return QVariant ();
    case 0:
      switch (role)
      {
      case Qt::DisplayRole:
        return "";
      case Qt::DecorationRole:
        return index.row () < m_layers_handler->layers_count ()  ? (m_layers_handler->is_layer_visible (index.row ()) ? eye_open_icon : eye_closed_icon) : QVariant ();
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
    return (int) m_layers_handler->layers_count () + 1;
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
  return layer_num != -1 ? index (layer_num, 0) : index (m_layers_handler->layers_count (), 0);
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
  switch (index.column ())
  {
  case 1: flags |= Qt::ItemIsEditable;
  }
  return flags;
}
