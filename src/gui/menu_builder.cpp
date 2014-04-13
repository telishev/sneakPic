#include "gui/menu_builder.h"

#include <QMenu>
#include <QMenuBar>

#include "gui/gui_actions.h"
#include "gui/gui_action_id.h"

menu_builder::menu_builder (QMenuBar *bar, const gui_actions *actions, QMenu *view_menu)
{
  m_bar = bar;
  m_actions = actions;
  m_view_menu = view_menu;
  update_menu ();
}

menu_builder::~menu_builder ()
{
}

void menu_builder::update_menu ()
{
  m_bar->clear ();
  create_file_menu (m_bar->addMenu ("File"));
  create_edit_menu (m_bar->addMenu ("Edit"));
  create_options_menu (m_bar->addMenu ("Options"));
  m_view_menu->setTitle ("View");
  m_bar->addMenu (m_view_menu);
}

void menu_builder::create_options_menu (QMenu *menu)
{
  add_action (menu, gui_action_id::OPTIONS);
}

void menu_builder::create_file_menu (QMenu *menu)
{
  add_action (menu, gui_action_id::NEW);
  add_action (menu, gui_action_id::OPEN);
  add_action (menu, gui_action_id::OPEN_RECENT);
  menu->addSeparator ();

  add_action (menu, gui_action_id::SAVE);
  add_action (menu, gui_action_id::SAVE_AS);
  menu->addSeparator ();
  add_action (menu, gui_action_id::EXPORT_BITMAP);
  menu->addSeparator ();
  add_action (menu, gui_action_id::QUIT);
}

void menu_builder::add_action (QMenu *menu, gui_action_id id)
{
  menu->addAction (m_actions->action (id));
}

void menu_builder::create_edit_menu (QMenu *menu)
{
  add_action (menu, gui_action_id::UNDO);
  add_action (menu, gui_action_id::REDO);
  menu->addSeparator ();
  add_action (menu, gui_action_id::CUT);
  add_action (menu, gui_action_id::COPY);
  add_action (menu, gui_action_id::PASTE);
  add_action (menu, gui_action_id::PASTE_IN_PLACE);
}

