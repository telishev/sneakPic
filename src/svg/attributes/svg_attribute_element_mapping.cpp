#include "svg/attributes/svg_attribute_element_mapping.h"

#include "common/common_utils.h"
#include "common/debug_utils.h"

#include "svg/attributes/svg_attribute_type.h"

#include "svg/items/svg_item_type.h"


#define INSERT_ITEM(ITEM) cur_map.insert (ITEM);

#define MAP_ATTRIBUTE(ATTRIBUTE,...)                        \
  do {                                                      \
    auto &cur_map = m_map[(int)ATTRIBUTE];                  \
    FOR_EACH (INSERT_ITEM, __VA_ARGS__)                     \
  } while (0);                                              \


svg_attribute_element_mapping::svg_attribute_element_mapping ()
{
  typedef svg_attribute_type s;
  typedef svg_item_type t;
  m_map.resize (enum_values_count (svg_attribute_type ()));
  fill_all_items (s::ID);

  fill_presentation_attribute (s::STYLE);
  fill_presentation_attribute (s::CLASS);
  fill_presentation_attribute (s::FILL);
  fill_presentation_attribute (s::STROKE);
  fill_presentation_attribute (s::STROKE_LINECAP);
  fill_presentation_attribute (s::STROKE_LINEJOIN);
  fill_presentation_attribute (s::STROKE_MITERLIMIT);
  fill_presentation_attribute (s::STROKE_WIDTH);
  fill_presentation_attribute (s::VISIBILITY);
  fill_presentation_attribute (s::DISPLAY);
  fill_presentation_attribute (s::OPACITY);
  fill_presentation_attribute (s::STROKE_OPACITY);
  fill_presentation_attribute (s::FILL_OPACITY);
  fill_presentation_attribute (s::STOP_COLOR);
  fill_presentation_attribute (s::STOP_OPACITY);
  fill_presentation_attribute (s::CLIP_PATH);
  fill_presentation_attribute (s::FILL_RULE);
  fill_presentation_attribute (s::CLIP_RULE);
  fill_presentation_attribute (s::FONT_FAMILY);
  fill_presentation_attribute (s::FONT_SIZE);
  fill_presentation_attribute (s::TEXT_ANCHOR);
  fill_presentation_attribute (s::COLOR);
  fill_presentation_attribute (s::STROKE_DASH_ARRAY);
  fill_presentation_attribute (s::STROKE_DASH_OFFSET);
  fill_presentation_attribute (s::MARKER_START);
  fill_presentation_attribute (s::MARKER_MID);
  fill_presentation_attribute (s::MARKER_END);
  fill_presentation_attribute (s::MARKER);

  MAP_ATTRIBUTE  (s::SVG_VERSION          , t::SVG);
  MAP_ATTRIBUTE  (s::WIDTH                , t::IMAGE, t::RECT, t::SVG, t::USE);
  MAP_ATTRIBUTE  (s::HEIGHT               , t::IMAGE, t::RECT, t::SVG, t::USE);
  MAP_ATTRIBUTE  (s::D                    , t::PATH);
  MAP_ATTRIBUTE  (s::TRANSFORM            , t::CIRCLE, t::CLIP_PATH, t::DEFS, t::ELLIPSE, t::G, t::IMAGE, t::LINE,
    t::PATH, t::POLYGON, t::POLYLINE, t::RECT, t::TEXT, t::USE);
  MAP_ATTRIBUTE  (s::X1                   , t::LINE, t::LINEAR_GRADIENT);
  MAP_ATTRIBUTE  (s::Y1                   , t::LINE, t::LINEAR_GRADIENT);
  MAP_ATTRIBUTE  (s::X2                   , t::LINE, t::LINEAR_GRADIENT);
  MAP_ATTRIBUTE  (s::Y2                   , t::LINE, t::LINEAR_GRADIENT);
  MAP_ATTRIBUTE  (s::X                    , t::IMAGE, t::RECT, t::SVG, t::TEXT, t::USE);
  MAP_ATTRIBUTE  (s::Y                    , t::IMAGE, t::RECT, t::SVG, t::TEXT, t::USE);
  MAP_ATTRIBUTE  (s::R                    , t::CIRCLE, t::RADIAL_GRADIENT);
  MAP_ATTRIBUTE  (s::RX                   , t::ELLIPSE, t::RECT);
  MAP_ATTRIBUTE  (s::RY                   , t::ELLIPSE, t::RECT);
  MAP_ATTRIBUTE  (s::CX                   , t::CIRCLE, t::ELLIPSE, t::RADIAL_GRADIENT);
  MAP_ATTRIBUTE  (s::CY                   , t::CIRCLE, t::ELLIPSE, t::RADIAL_GRADIENT);
  MAP_ATTRIBUTE  (s::FX                   , t::RADIAL_GRADIENT);
  MAP_ATTRIBUTE  (s::FY                   , t::RADIAL_GRADIENT);
  MAP_ATTRIBUTE  (s::HREF                 , t::RADIAL_GRADIENT, t::LINEAR_GRADIENT, t::IMAGE, t::USE);
  MAP_ATTRIBUTE  (s::GRADIENT_UNITS       , t::RADIAL_GRADIENT, t::LINEAR_GRADIENT);
  MAP_ATTRIBUTE  (s::SPREAD_METHOD        , t::RADIAL_GRADIENT, t::LINEAR_GRADIENT);
  MAP_ATTRIBUTE  (s::GRADIENT_TRANSFORM   , t::RADIAL_GRADIENT, t::LINEAR_GRADIENT);
  MAP_ATTRIBUTE  (s::OFFSET               , t::STOP);
  MAP_ATTRIBUTE  (s::VIEW_BOX             , t::SVG, t::MARKER);
  MAP_ATTRIBUTE  (s::POINTS               , t::POLYGON, t::POLYLINE);
  MAP_ATTRIBUTE  (s::DX                   , t::TEXT);
  MAP_ATTRIBUTE  (s::DY                   , t::TEXT);
  MAP_ATTRIBUTE  (s::PRESERVE_ASPECT_RATIO, t::IMAGE, t::SVG);
  MAP_ATTRIBUTE  (s::MARKER_UNITS         , t::MARKER);
  MAP_ATTRIBUTE  (s::MARKER_HEIGHT        , t::MARKER);
  MAP_ATTRIBUTE  (s::MARKER_WIDTH         , t::MARKER);
  MAP_ATTRIBUTE  (s::REFX                 , t::MARKER);
  MAP_ATTRIBUTE  (s::REFY                 , t::MARKER);
  MAP_ATTRIBUTE  (s::ORIENT               , t::MARKER);
  MAP_ATTRIBUTE  (s::MARKER_START         , t::PATH, t::LINE, t::POLYLINE, t::POLYGON);
  MAP_ATTRIBUTE  (s::MARKER_MID           , t::PATH, t::LINE, t::POLYLINE, t::POLYGON);
  MAP_ATTRIBUTE  (s::MARKER_END           , t::PATH, t::LINE, t::POLYLINE, t::POLYGON);
  MAP_ATTRIBUTE  (s::MARKER               , t::PATH, t::LINE, t::POLYLINE, t::POLYGON);
  MAP_ATTRIBUTE  (s::NODETYPES            , t::PATH);
  MAP_ATTRIBUTE  (s::LINETYPES            , t::PATH);
  

  check ();
}

svg_attribute_element_mapping::~svg_attribute_element_mapping ()
{

}

const svg_attribute_element_mapping *svg_attribute_element_mapping::get ()
{
  static const svg_attribute_element_mapping mapping;
  return &mapping;
}

bool svg_attribute_element_mapping::can_be_specified (svg_item_type item_type, svg_attribute_type attribute_type) const
{
  const auto &cur_map = m_map[(int)attribute_type];
  return cur_map.find (item_type) != cur_map.end ();
}

void svg_attribute_element_mapping::fill_presentation_attribute (svg_attribute_type type)
{
  typedef svg_item_type t;
  MAP_ATTRIBUTE (type, t::SVG, t::G, t::A, t::PATH, t::DEFS, t::LINE, t::RECT, t::ELLIPSE, t::CIRCLE, t::USE, t::STOP,
    t::LINEAR_GRADIENT, t::RADIAL_GRADIENT, t::CLIP_PATH, t::IMAGE, t::TEXT, t::POLYGON, t::POLYLINE, t::STYLE);
}

void svg_attribute_element_mapping::fill_all_items (svg_attribute_type type)
{
  for (int i = 0; i < (int)svg_item_type::COUNT; i++)
    m_map[(int)type].insert ((svg_item_type)i);
}

void svg_attribute_element_mapping::check ()
{
  for (size_t i = 0; i < m_map.size (); i++)
    if (m_map[i].empty ())
      DEBUG_PAUSE ("One of the attributes is empty");
}
