#include "gui/actions_info.h"

#include "gui/gui_action_id.h"


action_description::action_description (const QString &description, const QIcon &icon, const QKeySequence &shortcut)
  : m_icon (icon), m_description (description), m_default_shortcut (shortcut)
{
}

action_description::action_description ()
{

}

action_description::~action_description ()
{

}

actions_info::actions_info ()
{
  m_info.resize ((int)gui_action_id::COUNT);
  
  add_action (gui_action_id::OPEN, "Open...", QKeySequence::Open);
  add_action (gui_action_id::OPEN_RECENT, "Open Recent");
  add_action (gui_action_id::SAVE, "Save...", QKeySequence::Save);
  add_action (gui_action_id::SAVE_AS, "Save As...", QKeySequence::SaveAs);
  add_action (gui_action_id::UNDO, "Undo", QKeySequence::Undo);
  add_action (gui_action_id::REDO, "Redo", QKeySequence::Redo);

  /// Tools
  add_action (gui_action_id::TOOL_SELECTOR, "Selection Tool", QKeySequence (), QIcon (":/cursor.png"));
  
}

actions_info::~actions_info ()
{

}

const QIcon &actions_info::icon (gui_action_id id) const
{
  return descr (id).icon ();
}

const QString &actions_info::description (gui_action_id id) const
{
  return descr (id).description ();
}

const QKeySequence &actions_info::default_shortcut (gui_action_id id) const
{
  return descr (id).default_shortcut ();
}

void actions_info::add_action (gui_action_id id, const QString &description, QKeySequence shortcut, QIcon icon)
{
  m_info[(int)id] = action_description (description, icon, shortcut);
}

const action_description &actions_info::descr (gui_action_id id) const
{
  return m_info[(int)id];
}
