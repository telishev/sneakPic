#ifndef SHORTCUTS_CONFIG_H
#define SHORTCUTS_CONFIG_H

#include "gui/mouse_shortcut.h"

#include <QKeySequence>
#include <vector>

enum class gui_action_id;

enum class mouse_shortcut_enum
{
  SELECT_ITEM,
  ADD_ITEM_TO_SELECTION,
  FIND_CURRENT_OBJECT,
  HIGHLIGHT_HANDLE,
  SELECT_HANDLE,

  COUNT,
};

enum class mouse_drag_shortcut_enum
{
  PAN,
  DRAG_HANDLE,
  DRAG_OBJECTS,
  RUBBERBAND_SELECTION,
  CREATE_RECTANGLE,

  COUNT,
};

class shortcuts_config
{
  std::vector<mouse_shortcut> m_mouse_shortcuts;
  std::vector<mouse_shortcut> m_drag_shortcuts;
  std::vector<QKeySequence> m_action_shortcuts;

public:
  shortcuts_config ();
  ~shortcuts_config ();

  mouse_shortcut shortcut_mouse (mouse_shortcut_enum index) const { return m_mouse_shortcuts[(int)index]; }
  mouse_shortcut drag_shortcut_mouse (mouse_drag_shortcut_enum index) const { return m_drag_shortcuts[(int)index]; }
  QKeySequence action_shortcut (gui_action_id index) const { return m_action_shortcuts[(int)index]; }

  void fill_by_default ();

private:
  void fill_mouse_default ();
  void fill_drag_default ();
  void fill_action_default ();
};

#endif // SHORTCUTS_CONFIG_H

