#include "editor/tools/path_editor_tool.h"

#include "common/memory_deallocation.h"

#include "editor/items_selection.h"

#include "gui/mouse_shortcuts_handler.h"
#include "gui/settings.h"
#include "gui/actions_applier.h"
#include "gui/gui_action_id.h"

#include "renderer/svg_painter.h"
#include "renderer/rubberband_selection.h"
#include "renderer/overlay_renderer.h"

#include "svg/items/abstract_svg_item.h"
#include "svg/items/svg_items_container.h"

path_editor_tool::path_editor_tool (svg_painter *painter)
  : abstract_tool (painter)
{
  m_rubberband = new rubberband_selection (m_overlay, m_painter, m_mouse_handler);

  m_actions_applier->register_action (gui_action_id::DELETE_HANDLES, this, &path_editor_tool::delete_handles);
}

path_editor_tool::~path_editor_tool ()
{
  FREE (m_rubberband);
}

bool path_editor_tool::delete_handles ()
{
  return true;
}
