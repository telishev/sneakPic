#include "gui/actions_applier.h"

#include "common/debug_utils.h"

#include "gui/gui_action_id.h"
#include "shortcuts_config.h"


actions_applier::actions_applier ()
{
  m_actions.resize ((int)gui_action_id::COUNT);
}

actions_applier::~actions_applier ()
{

}

void actions_applier::register_action (gui_action_id id, action_type func)
{
  DEBUG_ASSERT (!m_actions[(int)id]);
  m_actions[(int)id] = func;
}

bool actions_applier::apply_action (gui_action_id id)
{
  if (!m_actions[(int)id])
    return false;

  return m_actions[(int)id] ();
}
