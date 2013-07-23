#include "svg/attributes/svg_attribute_element_mapping.h"

#include "common/common_utils.h"
#include "common/debug_utils.h"

#include "svg/attributes/attribute_type.h"

#include "svg/items/svg_item_type.h"


#define INSERT_ITEM(ITEM) cur_map.insert (svg_item_type::ITEM);

#define DO_MAP_ATTRIBUTE(ATTRIBUTE,...)                        \
  do {                                                         \
    auto &cur_map = m_map[(int)ATTRIBUTE];                     \
    FOR_EACH (INSERT_ITEM, __VA_ARGS__)                        \
  } while (0);                                                 \

#define MAP_ATTRIBUTE(ATTRIBUTE,...) DO_MAP_ATTRIBUTE (svg_attribute_type::ATTRIBUTE, __VA_ARGS__)

svg_attribute_element_mapping::svg_attribute_element_mapping ()
{
  m_map.resize (enum_values_count (svg_attribute_type ()));
  fill_all_items (svg_attribute_type::ID);

  fill_presentation_attribute (svg_attribute_type::STYLE);
  fill_presentation_attribute (svg_attribute_type::CLASS);
  fill_presentation_attribute (svg_attribute_type::FILL);
  fill_presentation_attribute (svg_attribute_type::STROKE);
  fill_presentation_attribute (svg_attribute_type::STROKE_LINECAP);
  fill_presentation_attribute (svg_attribute_type::STROKE_LINEJOIN);
  fill_presentation_attribute (svg_attribute_type::STROKE_MITERLIMIT);
  fill_presentation_attribute (svg_attribute_type::STROKE_WIDTH);
  fill_presentation_attribute (svg_attribute_type::VISIBILITY);
  fill_presentation_attribute (svg_attribute_type::DISPLAY);
  fill_presentation_attribute (svg_attribute_type::OPACITY);
  fill_presentation_attribute (svg_attribute_type::STROKE_OPACITY);
  fill_presentation_attribute (svg_attribute_type::FILL_OPACITY);
  fill_presentation_attribute (svg_attribute_type::STOP_COLOR);
  fill_presentation_attribute (svg_attribute_type::STOP_OPACITY);
  fill_presentation_attribute (svg_attribute_type::CLIP_PATH);
  fill_presentation_attribute (svg_attribute_type::FILL_RULE);
  fill_presentation_attribute (svg_attribute_type::CLIP_RULE);
  fill_presentation_attribute (svg_attribute_type::FONT_FAMILY);
  fill_presentation_attribute (svg_attribute_type::FONT_SIZE);
  fill_presentation_attribute (svg_attribute_type::TEXT_ANCHOR);

  MAP_ATTRIBUTE  (SVG_VERSION        , SVG);
  MAP_ATTRIBUTE  (WIDTH              , IMAGE, RECT, SVG, USE);
  MAP_ATTRIBUTE  (HEIGHT             , IMAGE, RECT, SVG, USE);
  MAP_ATTRIBUTE  (D                  , PATH);
  MAP_ATTRIBUTE  (TRANSFORM          , CIRCLE, CLIP_PATH, DEFS, ELLIPSE, G, IMAGE, LINE, PATH, POLYGON, POLYLINE, RECT, TEXT, USE);
  MAP_ATTRIBUTE  (X1                 , LINE, LINEAR_GRADIENT);
  MAP_ATTRIBUTE  (Y1                 , LINE, LINEAR_GRADIENT);
  MAP_ATTRIBUTE  (X2                 , LINE, LINEAR_GRADIENT);
  MAP_ATTRIBUTE  (Y2                 , LINE, LINEAR_GRADIENT);
  MAP_ATTRIBUTE  (X                  , IMAGE, RECT, SVG, TEXT, USE);
  MAP_ATTRIBUTE  (Y                  , IMAGE, RECT, SVG, TEXT, USE);
  MAP_ATTRIBUTE  (R                  , CIRCLE, RADIAL_GRADIENT);
  MAP_ATTRIBUTE  (RX                 , ELLIPSE, RECT);
  MAP_ATTRIBUTE  (RY                 , ELLIPSE, RECT);
  MAP_ATTRIBUTE  (CX                 , CIRCLE, ELLIPSE, RADIAL_GRADIENT);
  MAP_ATTRIBUTE  (CY                 , CIRCLE, ELLIPSE, RADIAL_GRADIENT);
  MAP_ATTRIBUTE  (FX                 , RADIAL_GRADIENT);
  MAP_ATTRIBUTE  (FY                 , RADIAL_GRADIENT);
  MAP_ATTRIBUTE  (HREF               , RADIAL_GRADIENT, LINEAR_GRADIENT, IMAGE, USE);
  MAP_ATTRIBUTE  (GRADIENT_UNITS     , RADIAL_GRADIENT, LINEAR_GRADIENT);
  MAP_ATTRIBUTE  (SPREAD_METHOD      , RADIAL_GRADIENT, LINEAR_GRADIENT);
  MAP_ATTRIBUTE  (GRADIENT_TRANSFORM , RADIAL_GRADIENT, LINEAR_GRADIENT);
  MAP_ATTRIBUTE  (OFFSET             , STOP);
  MAP_ATTRIBUTE  (VIEW_BOX           , SVG);
  MAP_ATTRIBUTE  (POINTS             , POLYGON, POLYLINE);
  MAP_ATTRIBUTE  (DX                 , TEXT);
  MAP_ATTRIBUTE  (DY                 , TEXT);

  check ();
}

svg_attribute_element_mapping::~svg_attribute_element_mapping ()
{

}

svg_attribute_element_mapping *svg_attribute_element_mapping::get ()
{
  static svg_attribute_element_mapping mapping;
  return &mapping;
}

bool svg_attribute_element_mapping::can_be_specified (svg_item_type item_type, svg_attribute_type attribute_type) const
{
  const auto &cur_map = m_map[(int)attribute_type];
  return cur_map.find (item_type) != cur_map.end ();
}

void svg_attribute_element_mapping::fill_presentation_attribute (svg_attribute_type type)
{
  DO_MAP_ATTRIBUTE (type, SVG, G, PATH, DEFS, LINE, RECT, ELLIPSE, CIRCLE, USE, STOP, LINEAR_GRADIENT, RADIAL_GRADIENT, CLIP_PATH, IMAGE, TEXT, POLYGON, POLYLINE);
}

void svg_attribute_element_mapping::fill_all_items (svg_attribute_type type)
{
#define DECLARE_ITEM(ENUM,NAME,NAMESPACE,CLASS) m_map[(int)type].insert (svg_item_type::ENUM);
  DECLARE_SVG_ITEM
#undef DECLARE_ITEM
}

void svg_attribute_element_mapping::check ()
{
  for (size_t i = 0; i < m_map.size (); i++)
    if (m_map[i].empty () && i != (int)svg_attribute_type::UNKNOWN)
      DEBUG_PAUSE ("One of the attributes is empty");
}
