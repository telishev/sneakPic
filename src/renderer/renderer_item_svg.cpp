#include "renderer/renderer_item_svg.h"

#include <QPainter>

#include "renderer/renderer_state.h"
#include "renderer/qt2skia.h"
#include "renderer/renderer_config.h"

#include "skia/skia_includes.h"


renderer_item_svg::renderer_item_svg (const string &name)
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
