#include "gui_actions.h"

#include <QAction>
#include <QSignalMapper>

#include "common/memory_deallocation.h"

#include "gui/actions_info.h"
#include "gui_action_id.h"
#include "shortcuts_config.h"


gui_actions::gui_actions (const shortcuts_config *cfg)
{
  m_cfg = cfg;
  m_info = new actions_info;
  m_mapper = nullptr;
  update_actions ();
}

gui_actions::~gui_actions ()
{
  FREE (m_info);
  FREE (m_mapper);
}

QAction *gui_actions::action (gui_action_id id) const
{
  return m_actions[(int)id].get ();
}

void gui_actions::update_actions ()
{
  m_actions.clear ();
  m_actions.resize ((int)gui_action_id::COUNT);
  FREE (m_mapper);
  m_mapper = new QSignalMapper (this);
  connect (m_mapper, SIGNAL (mapped (int)), this, SLOT (action_triggered (int)));

  for (int i = 0; i < (int)gui_action_id::COUNT; i++)
    {
      gui_action_id id = (gui_action_id)i;
      QAction *action = new QAction (m_info->icon (id), m_info->description (id), this);
      action->setShortcut (m_cfg->action_shortcut (id));
      connect (action, SIGNAL (triggered ()), m_mapper, SLOT (map ()));
      m_mapper->setMapping (action, i);
      m_actions[i].reset (action);
    }
}

void gui_actions::action_triggered (int id)
{
  emit triggered ((gui_action_id)id);
}

