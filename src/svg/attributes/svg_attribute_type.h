#ifndef ATTRIBUTE_TYPE_H
#define ATTRIBUTE_TYPE_H


/// !!!! Don't forget to update svg/attributes/svg_attribute_element_mapping.h after adding new attribute

#define DECLARE_SVG_ATTRIBUTES                                                                                                     \
/*                                                                                                                             */  \
/*                ENUM                 |   NAME               |  NAMESPACE |         CLASS                      | INHERIT_TYPE */  \
DECLARE_ATTRIBUTE(ID                   , "id"                 , SVG        , svg_attribute_id                   , NONE         )   \
DECLARE_ATTRIBUTE(SVG_VERSION          , "version"            , SVG        , svg_attribute_version              , NONE         )   \
DECLARE_ATTRIBUTE(WIDTH                , "width"              , SVG        , svg_attribute_width                , NONE         )   \
DECLARE_ATTRIBUTE(HEIGHT               , "height"             , SVG        , svg_attribute_height               , NONE         )   \
DECLARE_ATTRIBUTE(D                    , "d"                  , SVG        , svg_attribute_path_data            , NONE         )   \
DECLARE_ATTRIBUTE(STYLE                , "style"              , SVG        , svg_attribute_style                , NONE         )   \
DECLARE_ATTRIBUTE(CLASS                , "class"              , SVG        , svg_attribute_class                , NONE         )   \
DECLARE_ATTRIBUTE(FILL                 , "fill"               , SVG        , svg_attribute_fill                 , STYLE        )   \
DECLARE_ATTRIBUTE(STROKE               , "stroke"             , SVG        , svg_attribute_stroke               , STYLE        )   \
DECLARE_ATTRIBUTE(STROKE_LINECAP       , "stroke-linecap"     , SVG        , svg_attribute_stroke_linecap       , STYLE        )   \
DECLARE_ATTRIBUTE(STROKE_LINEJOIN      , "stroke-linejoin"    , SVG        , svg_attribute_stroke_linejoin      , STYLE        )   \
DECLARE_ATTRIBUTE(STROKE_MITERLIMIT    , "stroke-miterlimit"  , SVG        , svg_attribute_stroke_miterlimit    , STYLE        )   \
DECLARE_ATTRIBUTE(STROKE_WIDTH         , "stroke-width"       , SVG        , svg_attribute_stroke_width         , STYLE        )   \
DECLARE_ATTRIBUTE(TRANSFORM            , "transform"          , SVG        , svg_attribute_transform            , NONE         )   \
DECLARE_ATTRIBUTE(X1                   , "x1"                 , SVG        , svg_attribute_x1                   , NONE         )   \
DECLARE_ATTRIBUTE(Y1                   , "y1"                 , SVG        , svg_attribute_y1                   , NONE         )   \
DECLARE_ATTRIBUTE(X2                   , "x2"                 , SVG        , svg_attribute_x2                   , NONE         )   \
DECLARE_ATTRIBUTE(Y2                   , "y2"                 , SVG        , svg_attribute_y2                   , NONE         )   \
DECLARE_ATTRIBUTE(X                    , "x"                  , SVG        , svg_attribute_x                    , NONE         )   \
DECLARE_ATTRIBUTE(Y                    , "y"                  , SVG        , svg_attribute_y                    , NONE         )   \
DECLARE_ATTRIBUTE(R                    , "r"                  , SVG        , svg_attribute_r                    , NONE         )   \
DECLARE_ATTRIBUTE(RX                   , "rx"                 , SVG        , svg_attribute_rx                   , NONE         )   \
DECLARE_ATTRIBUTE(RY                   , "ry"                 , SVG        , svg_attribute_ry                   , NONE         )   \
DECLARE_ATTRIBUTE(CX                   , "cx"                 , SVG        , svg_attribute_cx                   , NONE         )   \
DECLARE_ATTRIBUTE(CY                   , "cy"                 , SVG        , svg_attribute_cy                   , NONE         )   \
DECLARE_ATTRIBUTE(FX                   , "fx"                 , SVG        , svg_attribute_fx                   , NONE         )   \
DECLARE_ATTRIBUTE(FY                   , "fy"                 , SVG        , svg_attribute_fy                   , NONE         )   \
DECLARE_ATTRIBUTE(VISIBILITY           , "visibility"         , SVG        , svg_attribute_visibility           , STYLE        )   \
DECLARE_ATTRIBUTE(DISPLAY              , "display"            , SVG        , svg_attribute_display              , STYLE        )   \
DECLARE_ATTRIBUTE(OPACITY              , "opacity"            , SVG        , svg_attribute_opacity              , STYLE        )   \
DECLARE_ATTRIBUTE(STROKE_OPACITY       , "stroke-opacity"     , SVG        , svg_attribute_stroke_opacity       , STYLE        )   \
DECLARE_ATTRIBUTE(FILL_OPACITY         , "fill-opacity"       , SVG        , svg_attribute_fill_opacity         , STYLE        )   \
DECLARE_ATTRIBUTE(HREF                 , "href"               , XLINK      , svg_attribute_xlink_href           , NONE         )   \
DECLARE_ATTRIBUTE(GRADIENT_UNITS       , "gradientUnits"      , SVG        , svg_attribute_gradient_units       , NONE         )   \
DECLARE_ATTRIBUTE(SPREAD_METHOD        , "spreadMethod"       , SVG        , svg_attribute_spread_method        , NONE         )   \
DECLARE_ATTRIBUTE(GRADIENT_TRANSFORM   , "gradientTransform"  , SVG        , svg_attribute_gradient_transform   , NONE         )   \
DECLARE_ATTRIBUTE(OFFSET               , "offset"             , SVG        , svg_attribute_offset               , NONE         )   \
DECLARE_ATTRIBUTE(STOP_COLOR           , "stop-color"         , SVG        , svg_attribute_stop_color           , STYLE        )   \
DECLARE_ATTRIBUTE(STOP_OPACITY         , "stop-opacity"       , SVG        , svg_attribute_stop_opacity         , STYLE        )   \
DECLARE_ATTRIBUTE(CLIP_PATH            , "clip-path"          , SVG        , svg_attribute_clip_path            , NONE         )   \
DECLARE_ATTRIBUTE(FILL_RULE            , "fill-rule"          , SVG        , svg_attribute_fill_rule            , STYLE        )   \
DECLARE_ATTRIBUTE(CLIP_RULE            , "clip-rule"          , SVG        , svg_attribute_clip_rule            , STYLE        )   \
DECLARE_ATTRIBUTE(VIEW_BOX             , "viewBox"            , SVG        , svg_attribute_view_box             , NONE         )   \
DECLARE_ATTRIBUTE(FONT_FAMILY          , "font-family"        , SVG        , svg_attribute_font_family          , STYLE        )   \
DECLARE_ATTRIBUTE(FONT_SIZE            , "font-size"          , SVG        , svg_attribute_font_size            , STYLE        )   \
DECLARE_ATTRIBUTE(POINTS               , "points"             , SVG        , svg_attribute_points               , NONE         )   \
DECLARE_ATTRIBUTE(DX                   , "dx"                 , SVG        , svg_attribute_dx                   , NONE         )   \
DECLARE_ATTRIBUTE(DY                   , "dy"                 , SVG        , svg_attribute_dy                   , NONE         )   \
DECLARE_ATTRIBUTE(TEXT_ANCHOR          , "text-anchor"        , SVG        , svg_attribute_text_anchor          , STYLE        )   \
DECLARE_ATTRIBUTE(COLOR                , "color"              , SVG        , svg_attribute_color                , STYLE        )   \
DECLARE_ATTRIBUTE(STROKE_DASH_ARRAY    , "stroke-dasharray"   , SVG        , svg_attribute_stroke_dash_array    , STYLE        )   \
DECLARE_ATTRIBUTE(STROKE_DASH_OFFSET   , "stroke-dashoffset"  , SVG        , svg_attribute_stroke_dash_offset   , STYLE        )   \
DECLARE_ATTRIBUTE(PRESERVE_ASPECT_RATIO, "preserveAspectRatio", SVG        , svg_attribute_preserve_aspect_ratio, NONE         )   \
DECLARE_ATTRIBUTE(MARKER_UNITS         , "markerUnits"        , SVG        , svg_attribute_marker_units         , NONE         )   \
DECLARE_ATTRIBUTE(REFX                 , "refX"               , SVG        , svg_attribute_refx                 , NONE         )   \
DECLARE_ATTRIBUTE(REFY                 , "refY"               , SVG        , svg_attribute_refy                 , NONE         )   \
DECLARE_ATTRIBUTE(MARKER_WIDTH         , "markerWidth"        , SVG        , svg_attribute_marker_width         , NONE         )   \
DECLARE_ATTRIBUTE(MARKER_HEIGHT        , "markerHeight"       , SVG        , svg_attribute_marker_height        , NONE         )   \
DECLARE_ATTRIBUTE(ORIENT               , "orient"             , SVG        , svg_attribute_orient               , NONE         )   \
DECLARE_ATTRIBUTE(MARKER_START         , "marker-start"       , SVG        , svg_attribute_marker_start         , STYLE        )   \
DECLARE_ATTRIBUTE(MARKER_MID           , "marker-mid"         , SVG        , svg_attribute_marker_mid           , STYLE        )   \
DECLARE_ATTRIBUTE(MARKER_END           , "marker-end"         , SVG        , svg_attribute_marker_end           , STYLE        )   \
DECLARE_ATTRIBUTE(MARKER               , "marker"             , SVG        , svg_attribute_marker               , STYLE        )   \
DECLARE_ATTRIBUTE(NODETYPES            , "nodetypes"          , SNEAKPIC   , svg_attribute_nodetypes            , STYLE        )   \
DECLARE_ATTRIBUTE(LINETYPES            , "linetypes"          , SNEAKPIC   , svg_attribute_linetypes            , STYLE        )   \

enum class svg_attribute_type
{
  ID,
  SVG_VERSION,
  WIDTH,
  HEIGHT,
  D,
  STYLE,
  CLASS,
  FILL,
  STROKE,
  STROKE_LINECAP,
  STROKE_LINEJOIN,
  STROKE_MITERLIMIT,
  STROKE_WIDTH,
  TRANSFORM,
  X1,
  Y1,
  X2,
  Y2,
  X,
  Y,
  R,
  RX,
  RY,
  CX,
  CY,
  FX,
  FY,
  VISIBILITY,
  DISPLAY,
  OPACITY,
  STROKE_OPACITY,
  FILL_OPACITY,
  HREF,
  GRADIENT_UNITS,
  SPREAD_METHOD,
  GRADIENT_TRANSFORM,
  OFFSET,
  STOP_COLOR,
  STOP_OPACITY,
  CLIP_PATH,
  FILL_RULE,
  CLIP_RULE,
  VIEW_BOX,
  FONT_FAMILY,
  FONT_SIZE,
  POINTS,
  DX,
  DY,
  TEXT_ANCHOR,
  COLOR,
  STROKE_DASH_ARRAY,
  STROKE_DASH_OFFSET,
  PRESERVE_ASPECT_RATIO,
  MARKER_UNITS,
  REFX,
  REFY,
  MARKER_WIDTH,
  MARKER_HEIGHT,
  ORIENT,
  MARKER_START,
  MARKER_MID,
  MARKER_END,
  MARKER,
  NODETYPES,
  LINETYPES,

  COUNT,
};

static inline int enum_values_count (svg_attribute_type)
{
  return (int)svg_attribute_type::COUNT;
}

enum class svg_inherit_type
{
  NONE,
  INHERIT,
  STYLE,
};

#endif // ATTRIBUTE_TYPE_H

