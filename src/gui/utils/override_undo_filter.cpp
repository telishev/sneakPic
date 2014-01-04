#include "override_undo_filter.h"

#include <QEvent>
#include <QWidget>

bool override_undo_filter::eventFilter (QObject *target, QEvent *anEvent)
{
  const QEvent::Type type = anEvent->type();
  switch (type)
    {
    case QEvent::ShortcutOverride:
      return true;
    default:
      break;
    }
  return m_parent->eventFilter (target, anEvent);
}

override_undo_filter::override_undo_filter (QWidget *parent) : QObject (parent)
{
  m_parent = parent;
}
