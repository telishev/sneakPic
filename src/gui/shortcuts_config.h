#ifndef SHORTCUTS_CONFIG_H
#define SHORTCUTS_CONFIG_H

#include "gui/mouse_shortcut.h"

#include <QKeySequence>
#include <vector>

enum class gui_action_id;

/// Higher actions have higher priority
enum class mouse_shortcut_t
{
  CHANGE_HANDLE_TYPE,
  PEN_ADD_SEGMENT_SIMPLE,
  ADD_PATH_POINT,
  UPDATE_AUXILIARY_PEN_PREVIEW,
  HIGHLIGHT_HANDLE,
  FIND_CURRENT_OBJECT,
  SELECT_HANDLE,
  SELECT_PREV_ITEM,
  SELECT_ITEM,
  SWITCH_SELECTOR_HANDLES_TYPE,
  COLOR_PICKER_CLICK,
  ADD_GRADIENT_STOP,

  COUNT,
};

enum class mouse_drag_shortcut_t
{
  PENCIL,
  PEN_ADD_SEGMENT_DRAG,
  CREATE_RECTANGLE,
  CREATE_ELLIPSE,
  PAN,
  DRAG_HANDLE,
  DRAG_OBJECTS,
  HANDLES_SELECTION,
  RUBBERBAND_SELECTION,
  COLOR_PICKER_DRAG,

  COUNT,
};

class shortcuts_config
{
  vector<mouse_shortcut> m_mouse_shortcuts;
  vector<mouse_shortcut> m_drag_shortcuts;
  vector<QKeySequence> m_action_shortcuts;

public:
  shortcuts_config ();
  ~shortcuts_config ();

  mouse_shortcut shortcut_mouse (mouse_shortcut_t index) const { return m_mouse_shortcuts[(int)index]; }
  mouse_shortcut drag_shortcut_mouse (mouse_drag_shortcut_t index) const { return m_drag_shortcuts[(int)index]; }
  QKeySequence action_shortcut (gui_action_id index) const { return m_action_shortcuts[(int)index]; }

  void fill_by_default ();

private:
  void fill_mouse_default ();
  void fill_drag_default ();
  void fill_action_default ();
  void set_mouse_shortcut (mouse_shortcut_t index, mouse_event_type type, mouse_button button, keyboard_modifiers modifiers);
  void set_mouse_drag_shortcut (mouse_drag_shortcut_t index, mouse_button button, keyboard_modifiers modifiers);
};

#endif // SHORTCUTS_CONFIG_H

