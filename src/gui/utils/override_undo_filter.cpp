#include "override_undo_filter.h"

#include <QKeyEvent>
#include <QWidget>

bool override_undo_filter::eventFilter (QObject *target, QEvent *anEvent)
{
  const QEvent::Type type = anEvent->type();
  switch (type)
    {
    case QEvent::ShortcutOverride:
      {
        QKeyEvent *key_event = static_cast <QKeyEvent *> (anEvent);
        if (key_event->matches (QKeySequence::Undo) || key_event->matches (QKeySequence::Redo))
          return true;
      }
    default:
      break;
    }
  return m_parent->eventFilter (target, anEvent);
}

override_undo_filter::override_undo_filter (QWidget *parent) : QObject (parent)
{
  m_parent = parent;
}
