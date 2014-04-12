#include "editor/tools/path_editor_tool.h"

#include "common/memory_deallocation.h"

#include "editor/items_selection.h"
#include "editor/path_handles_editor.h"
#include "editor/gradient_handles_editor.h"

#include "gui/mouse_shortcuts_handler.h"
#include "gui/settings.h"
#include "gui/actions_applier.h"
#include "gui/gui_action_id.h"

#include "renderer/svg_painter.h"
#include "renderer/overlay_renderer.h"
#include "renderer/items_rubberband_selector.h"

#include "svg/items/abstract_svg_item.h"
#include "svg/items/svg_items_container.h"


path_editor_tool::path_editor_tool (svg_painter *painter)
  : abstract_tool (painter)
{
  m_path_editor.reset (new path_handles_editor (m_overlay, m_painter, m_actions_applier));
  m_rubberband.reset (new items_rubberband_selector (m_overlay, m_painter, m_actions_applier));
  m_gradient_editor.reset (new gradient_handles_editor (m_overlay, m_painter, m_actions_applier));
}

path_editor_tool::~path_editor_tool ()
{
}

void path_editor_tool::configure ()
{
  if (m_painter->get_configure_needed (configure_type::SELECTION_CHANGED))
    {
      update_handles ();
    }
}

void path_editor_tool::activate ()
{
  update_handles ();
  m_gradient_editor->set_page_active (true);
}

void path_editor_tool::update_handles ()
{
  m_path_editor->update_handles ();
  m_gradient_editor->update_handles ();
}

void path_editor_tool::deactivate ()
{
  m_gradient_editor->set_page_active (false);
}
