#include "renderer/renderer_item_svg.h"

#include <QPainter>

#include "renderer/renderer_state.h"
#include "renderer/qt2skia.h"
#include "renderer/renderer_config.h"

#pragma warning(push, 0)
#include <SkCanvas.h>
#include <SkSurface.h>
#include <SkDevice.h>
#pragma warning(pop)


renderer_item_svg::renderer_item_svg (const std::string &name)
  : renderer_item_group (name)
{
}

renderer_item_svg::~renderer_item_svg ()
{

}

void renderer_item_svg::draw (SkCanvas &canvas, const renderer_state &state, const renderer_config *config) const
{
  renderer_item_group::draw (canvas, state, config);
}
