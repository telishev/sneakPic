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
  typedef gui_action_id A;
  m_info.resize ((int) A::COUNT);

  add_action (A::NEW, "New", QKeySequence::New);
  add_action (A::OPEN, "Open...", QKeySequence::Open);
  add_action (A::OPEN_RECENT, "Open Recent");
  add_action (A::SAVE, "Save", QKeySequence::Save);
  add_action (A::SAVE_AS, "Save As...", QKeySequence::SaveAs);
  add_action (A::EXPORT_BITMAP, "Export Bitmap...", QKeySequence ("Ctrl+E"));
  add_action (A::UNDO, "Undo", QKeySequence::Undo);
  add_action (A::REDO, "Redo", QKeySequence::Redo);
  add_action (A::COPY, "Copy", QKeySequence::Copy);
  add_action (A::PASTE, "Paste", QKeySequence::Paste);
  add_action (A::PASTE_IN_PLACE, "Paste in Place", QKeySequence ("Ctrl+Shift+V"));
  add_action (A::CUT, "Cut", QKeySequence::Cut);
  add_action (A::OPTIONS, "Options...", QKeySequence ());
  add_action (A::QUIT, "Exit", QKeySequence ("Alt+F4"));

  /// Tools
  add_action (A::TOOL_SELECTOR, "Select Objects", QKeySequence (), QIcon (":/cursor.png"));
  add_action (A::TOOL_PATH_EDITOR, "Edit Paths by Nodes", QKeySequence (), QIcon (":/cursor.png"));
  add_action (A::TOOL_RECTANGLE, "Create Rectangles", QKeySequence (), QIcon (":/rectangle.png"));
  add_action (A::TOOL_ELLIPSE, "Create Ellipses", QKeySequence (), QIcon (":/ellipse.png"));
  add_action (A::TOOL_PEN, "Create Bezier Curves", QKeySequence (), QIcon (":/pen.png"));
  add_action (A::TOOL_PENCIL, "Create Freehand Lines", QKeySequence (), QIcon (":/pencil.png"));

  /// Other
  add_action (A::CANCEL_CURVE, "Cancel Curve", QKeySequence (Qt::Key_Delete));
  add_action (A::DELETE_HANDLES, "Delete Handles", QKeySequence (Qt::Key_Delete));
  add_action (A::DELETE_ITEMS, "Delete", QKeySequence (Qt::Key_Delete));
  add_action (A::FINISH_PATH, "Finish Path", QKeySequence (Qt::Key_Return));
  add_action (A::CANCEL_EDITING, "Cancel Editing", QKeySequence (Qt::Key_Escape));
  add_action (A::LOWER_OBJECT, "Lower Object", QKeySequence (Qt::Key_PageDown));
  add_action (A::RAISE_OBJECT, "Raise Object", QKeySequence (Qt::Key_PageUp));
  add_action (A::SWITCH_FILL_STROKE, "Switch Fill and Stroke", QKeySequence ("X"), QIcon (":/switch.png"));
  add_action (A::DESELECT_HANDLES, "Deselect Handles", QKeySequence (Qt::Key_Escape));
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
