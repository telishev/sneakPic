#include "gui/layers_tree_model.h"
#include "svg/layers_handler.h"
#include "gui/connection.h"

#include <QIcon>
#include <QMimeData>

#include "common/common_utils.h"
#include "svg/layers_tree.h"

#include <stack>

#include "svg/attributes/svg_attributes_enum.h"


enum class columnRole
{
  TYPE = 0,
  VISIBILITY,
  TITLE,
  OPACITY,

  COUNT
};

QVariant layers_tree_model::data (const QModelIndex &index, int role /*= Qt::DisplayRole*/) const
{
  switch ((columnRole) index.column ())
    {
    case columnRole::TITLE:
      switch (role)
        {
        case Qt::SizeHintRole:
          return QSize (200, 0);
        case Qt::DisplayRole:
        case Qt::EditRole:
          return m_layers_handler->get_layer_name (index);
        }
      return QVariant ();
    case columnRole::OPACITY:
      switch (role)
        {
        case Qt::DisplayRole:
        case Qt::EditRole:
          return QString::number (m_layers_handler->get_layer_opacity (index)) + "%";
        }
      return QVariant ();
    case columnRole::VISIBILITY:
      switch (role)
        {
        case Qt::DisplayRole:
          return "";
        case Qt::DecorationRole:
          return m_layers_handler->is_layer_visible (index) ? eye_open_icon : eye_closed_icon;
        }
      return QVariant ();
    case columnRole::TYPE:
      switch (role)
        {
        case Qt::DisplayRole:
          return "";
        case Qt::DecorationRole:
          return m_layers_handler->get_layer_type (index, layer_type::FOLDER) == layer_type::LAYER ? layer_icon : folder_icon;
        }
      return QVariant ();
    case columnRole::COUNT:
      return  QVariant ();
    }
  return QVariant ();
}

layers_tree_model::layers_tree_model ()
{
  m_layers_handler = 0;
  eye_closed_icon = QIcon (":/eye_closed.png");
  eye_open_icon = QIcon (":/eye_open.png");
  layer_icon = QIcon (":/layer.png");
  folder_icon = QIcon (":/folder.png");
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

  auto parent_node = static_cast <layers_tree_node *> (parent.internalPointer ());
  return parent_node != nullptr ? (int) parent_node->children.size () : (int) m_layers_handler->tree_root ()->children.size () ;
}

QModelIndex layers_tree_model::index (int row, int column, const QModelIndex &parent /*= QModelIndex()*/) const
{
  if (!m_layers_handler)
    return QModelIndex ();

  auto parent_node = static_cast <layers_tree_node *> (parent.internalPointer ());
  if (parent_node == nullptr)
    parent_node = m_layers_handler->tree_root ();
  return hasIndex (row, column, parent) ? createIndex (row, column, static_cast <void *> (parent_node->children[row].get ())) : QModelIndex();
}

int layers_tree_model::columnCount (const QModelIndex &/*parent = QModelIndex()*/) const
{
  return (int) columnRole::COUNT;
}

QModelIndex layers_tree_model::index_for_node (layers_tree_node *node) const
{
  std::stack<int> indexes;
  while (node->parent != nullptr)
    {
      indexes.push (node->node_number);
      node = node->parent;
    }

  QModelIndex curIndex = index (indexes.top (), 0);
  indexes.pop ();
  while (!indexes.empty ())
    {
      curIndex = curIndex.child (indexes.top (), 0);
      indexes.pop ();
    }

  return curIndex;
}

void layers_tree_model::set_layers_handler (layers_handler *handler)
{
  m_layers_handler = handler;
  update_model ();
}

bool layers_tree_model::setData (const QModelIndex &index, const QVariant &value, int role /*= Qt::EditRole */)
{
  switch (role)
    {
    case Qt::EditRole:
      switch ((columnRole) index.column ())
        {
        case columnRole::TITLE:
          m_layers_handler->rename_layer (index, value.toString ());
          emit resize_needed ();
          return true;
        default:
          return false; // TODO: probably add editing of opacity
        }
    }
  return false;
}

Qt::ItemFlags layers_tree_model::flags (const QModelIndex &index) const
{
  auto flags = QAbstractItemModel::flags (index);

  // if (index.column () == (int) columnRole::VISIBILITY)
  if (m_layers_handler->get_layer_type (index, layer_type::FOLDER) == layer_type::FOLDER)
    flags |= Qt::ItemIsDropEnabled;

  switch ((columnRole) index.column ())
  {
    case columnRole::TITLE:
      flags |= (Qt::ItemIsEditable | Qt::ItemIsDragEnabled);
      break;
    default:
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

bool layers_tree_model::dropMimeData (const QMimeData *data, Qt::DropAction /*action*/, int row, int column, const QModelIndex &parent)
{
  if (row >= 0 && column >= 0)
    {
      if (row < rowCount (parent))
        m_layers_handler->move_layer (reinterpret_cast<layers_tree_node *> (data->data (format).toLongLong ()), static_cast<layers_tree_node *> (index (row, column, parent).internalPointer ()));
      else
        m_layers_handler->move_layer (reinterpret_cast<layers_tree_node *> (data->data (format).toLongLong ()), static_cast<layers_tree_node *> (parent.internalPointer ()), true);
    }
  else
    m_layers_handler->move_layer_inside (reinterpret_cast<layers_tree_node *> (data->data (format).toLongLong ()), static_cast<layers_tree_node *> (parent.internalPointer ()));
  return true;
}

QMimeData *layers_tree_model::mimeData (const QModelIndexList &indexes) const
{
  // For our situation it's always 1 index, we need to pack row num
  QMimeData *data = new QMimeData ();
  data->setData (format, QByteArray::number ((long long) indexes[0].internalPointer ()));
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

QModelIndex layers_tree_model::parent (const QModelIndex &index) const
{
  auto node = static_cast<layers_tree_node *> (index.internalPointer ());
  if (node == nullptr)
    return createIndex (-1, -1, m_layers_handler->tree_root ());
  else
    return createIndex (node->node_number, 0, node->parent);
}

void layers_tree_model::index_clicked (const QModelIndex &index) const
{
  switch ((columnRole) index.column ())
    {
    case columnRole::VISIBILITY:
      m_layers_handler->toggle_layer_visibility (index);
      break;
    default:
      break;
    }
}
