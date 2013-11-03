#include "gui/gui_actions.h"

#include <QAction>
#include <QTimer>
#include <QShortcut>
#include <QShortcutEvent>

#include "common/memory_deallocation.h"

#include "gui/actions_info.h"
#include "gui_action_id.h"
#include "gui/shortcuts_config.h"
#include "gui/connection.h"


gui_actions::gui_actions (const shortcuts_config *cfg, std::function<bool (gui_action_id)> callback, QWidget *parent)
{
  m_parent = parent;
  m_cfg = cfg;
  m_info = new actions_info;
  m_callback = callback;
  m_actions_mapper = new QSignalMapper (this);
  connect (m_actions_mapper, SIGNAL (mapped (int)), this, SLOT (action_triggered (int)));

  m_actions.resize ((int)gui_action_id::COUNT);
  for (int i = 0; i < (int)gui_action_id::COUNT; i++)
    {
      QAction *act = new QAction (m_parent);
      m_parent->addAction (act);
      act->installEventFilter (this);
      m_actions[i].reset (act);
      CONNECT (act, SIGNAL (triggered ()), m_actions_mapper, SLOT (map ()));
      m_actions_mapper->setMapping (act, i);
    }

  rebuild_actions ();
}

gui_actions::~gui_actions ()
{
  FREE (m_info);
  FREE (m_actions_mapper);
}

QAction *gui_actions::action (gui_action_id id) const
{
  return m_actions[(int)id].get ();
}

void gui_actions::rebuild_actions ()
{
  for (int i = 0; i < (int)gui_action_id::COUNT; i++)
    {
      gui_action_id id = (gui_action_id)i;
      QKeySequence sequence = m_cfg->action_shortcut (id);
      QAction *action = m_actions[i].get ();
      action->setIcon (m_info->icon (id));
      action->setText (m_info->description (id));
      action->setShortcut (sequence);
    }
}

void gui_actions::set_update_needed ()
{
  emit actions_update_needed ();
}

void gui_actions::action_triggered (int id)
{
  m_callback ((gui_action_id)id);
}

void gui_actions::shortcut_triggered (QKeySequence sequnce)
{
  for (int i = 0; i < (int)gui_action_id::COUNT; i++)
    {
      gui_action_id id = (gui_action_id)i;
      if (m_cfg->action_shortcut (id) != sequnce)
        continue;

      if (m_callback (id))
        return;
    }
}

bool gui_actions::eventFilter (QObject *watched, QEvent *qevent)
{
  if (qevent->type () == QEvent::Shortcut)
    {
      QShortcutEvent *shortcut_ev = static_cast<QShortcutEvent *> (qevent);
      shortcut_triggered (shortcut_ev->key ());
      return true;
    }

  return QObject::eventFilter (watched, qevent);
}

