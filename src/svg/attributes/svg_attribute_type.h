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
DECLARE_ATTRIBUTE(FONT_FAMILY          , "font-family"        , SVG        , svg_attribute_font_family          , INHERIT      )   \
DECLARE_ATTRIBUTE(FONT_SIZE            , "font-size"          , SVG        , svg_attribute_font_size            , INHERIT      )   \
DECLARE_ATTRIBUTE(POINTS               , "points"             , SVG        , svg_attribute_points               , NONE         )   \
DECLARE_ATTRIBUTE(DX                   , "dx"                 , SVG        , svg_attribute_dx                   , NONE         )   \
DECLARE_ATTRIBUTE(DY                   , "dy"                 , SVG        , svg_attribute_dy                   , NONE         )   \
DECLARE_ATTRIBUTE(TEXT_ANCHOR          , "text-anchor"        , SVG        , svg_attribute_text_anchor          , INHERIT      )   \
DECLARE_ATTRIBUTE(COLOR                , "color"              , SVG        , svg_attribute_color                , STYLE        )   \
DECLARE_ATTRIBUTE(STROKE_DASH_ARRAY    , "stroke-dasharray"   , SVG        , svg_attribute_stroke_dash_array    , STYLE        )   \
DECLARE_ATTRIBUTE(STROKE_DASH_OFFSET   , "stroke-dashoffset"  , SVG        , svg_attribute_stroke_dash_offset   , STYLE        )   \
DECLARE_ATTRIBUTE(PRESERVE_ASPECT_RATIO, "preserveAspectRatio", SVG        , svg_attribute_preserve_aspect_ratio, NONE         )   \

enum class svg_attribute_type
{
  UNKNOWN,
#define DECLARE_ATTRIBUTE(ENUM,NAME,NAMESPACE,CLASS,INHERIT_TYPE) ENUM,
  DECLARE_SVG_ATTRIBUTES
#undef DECLARE_ATTRIBUTE

};

static inline int enum_values_count (svg_attribute_type)
{
#define DECLARE_ATTRIBUTE(ENUM,NAME,NAMESPACE,CLASS,INHERIT_TYPE) +1
  return DECLARE_SVG_ATTRIBUTES + 1;
#undef DECLARE_ATTRIBUTE
}

enum class svg_inherit_type
{
  NONE,
  INHERIT,
  STYLE,
};

#endif // ATTRIBUTE_TYPE_H

