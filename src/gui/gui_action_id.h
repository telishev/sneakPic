#ifndef GUI_ACTION_ID_H
#define GUI_ACTION_ID_H

/// Higher actions have higher priority
enum class gui_action_id
{

  //// Menu
  OPEN,
  OPEN_RECENT,
  SAVE,
  SAVE_AS,
  QUIT,
  UNDO,
  REDO,


  //// Tools
  TOOL_SELECTOR,
  TOOL_PATH_EDITOR,
  TOOL_RECTANGLE,

  /// Other
  DELETE_HANDLES,
  DELETE_ITEMS,

  /// Count
  COUNT,
};

#endif // GUI_ACTION_ID_H
