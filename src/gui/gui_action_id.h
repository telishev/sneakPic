#ifndef GUI_ACTION_ID_H
#define GUI_ACTION_ID_H

/// Higher actions have higher priority
enum class gui_action_id
{

  //// Menu
  NEW,
  OPEN,
  OPEN_RECENT,
  SAVE,
  SAVE_AS,
  QUIT,
  UNDO,
  REDO,
  COPY,
  PASTE,
  PASTE_IN_PLACE,
  CUT,
  OPTIONS,


  //// Tools
  TOOL_SELECTOR,
  TOOL_PATH_EDITOR,
  TOOL_RECTANGLE,
  TOOL_ELLIPSE,
  TOOL_PEN,

  /// Other
  CANCEL_CURVE,
  DELETE_HANDLES,
  DELETE_ITEMS,
  FINISH_PATH,
  CANCEL_EDITING,
  LOWER_OBJECT,
  RAISE_OBJECT,
  SWITCH_FILL_STROKE,
  DESELECT_HANDLES,

  INTERRUPT,

  /// Count
  COUNT,
};

#endif // GUI_ACTION_ID_H
