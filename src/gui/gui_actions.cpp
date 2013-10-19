#include "gui/gui_actions.h"

#include <QAction>
#include <QTimer>

#include "common/memory_deallocation.h"

#include "gui/actions_info.h"
#include "gui_action_id.h"
#include "gui/shortcuts_config.h"


gui_actions::gui_actions (const shortcuts_config *cfg)
{
  m_cfg = cfg;
  m_info = new actions_info;

  m_actions.resize ((int)gui_action_id::COUNT);
  for (int i = 0; i < (int)gui_action_id::COUNT; i++)
    m_actions[i].reset (new QAction (this));

  rebuild_actions ();
}

gui_actions::~gui_actions ()
{
  FREE (m_info);
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
      QAction *action = m_actions[i].get ();
      action->setIcon (m_info->icon (id));
      action->setText (m_info->description (id));
      action->setShortcut (m_cfg->action_shortcut (id));
    }
}

void gui_actions::set_update_needed ()
{
  emit actions_update_needed ();
}

