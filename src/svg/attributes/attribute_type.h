#ifndef ATTRIBUTE_TYPE_H
#define ATTRIBUTE_TYPE_H

#define DECLARE_SVG_ATTRIBUTES                                                                                            \
/*                                                                                                                    */  \
/*                ENUM              |   NAME              |  NAMESPACE |         CLASS                    | INHERIT_TYPE */  \
DECLARE_ATTRIBUTE(ID                , "id"                , EMPTY      , svg_attribute_id                 , NONE         )   \
DECLARE_ATTRIBUTE(VERSION           , "version"           , EMPTY      , svg_attribute_version            , NONE         )   \
DECLARE_ATTRIBUTE(WIDTH             , "width"             , EMPTY      , svg_attribute_width              , NONE         )   \
DECLARE_ATTRIBUTE(HEIGHT            , "height"            , EMPTY      , svg_attribute_height             , NONE         )   \
DECLARE_ATTRIBUTE(D                 , "d"                 , EMPTY      , svg_attribute_path_data          , NONE         )   \
DECLARE_ATTRIBUTE(STYLE             , "style"             , EMPTY      , svg_attribute_style              , NONE         )   \
DECLARE_ATTRIBUTE(CLASS             , "class"             , EMPTY      , svg_attribute_class              , NONE         )   \
DECLARE_ATTRIBUTE(FILL              , "fill"              , EMPTY      , svg_attribute_fill               , STYLE        )   \
DECLARE_ATTRIBUTE(STROKE            , "stroke"            , EMPTY      , svg_attribute_stroke             , STYLE        )   \
DECLARE_ATTRIBUTE(STROKE_LINECAP    , "stroke-linecap"    , EMPTY      , svg_attribute_stroke_linecap     , STYLE        )   \
DECLARE_ATTRIBUTE(STROKE_LINEJOIN   , "stroke-linejoin"   , EMPTY      , svg_attribute_stroke_linejoin    , STYLE        )   \
DECLARE_ATTRIBUTE(STROKE_MITERLIMIT , "stroke-miterlimit" , EMPTY      , svg_attribute_stroke_miterlimit  , STYLE        )   \
DECLARE_ATTRIBUTE(STROKE_WIDTH      , "stroke-width"      , EMPTY      , svg_attribute_stroke_width       , STYLE        )   \
DECLARE_ATTRIBUTE(TRANSFORM         , "transform"         , EMPTY      , svg_attribute_transform          , INHERIT      )   \
DECLARE_ATTRIBUTE(X1                , "x1"                , EMPTY      , svg_attribute_x1                 , NONE         )   \
DECLARE_ATTRIBUTE(Y1                , "y1"                , EMPTY      , svg_attribute_y1                 , NONE         )   \
DECLARE_ATTRIBUTE(X2                , "x2"                , EMPTY      , svg_attribute_x2                 , NONE         )   \
DECLARE_ATTRIBUTE(Y2                , "y2"                , EMPTY      , svg_attribute_y2                 , NONE         )   \
DECLARE_ATTRIBUTE(X                 , "x"                 , EMPTY      , svg_attribute_x                  , NONE         )   \
DECLARE_ATTRIBUTE(Y                 , "y"                 , EMPTY      , svg_attribute_y                  , NONE         )   \
DECLARE_ATTRIBUTE(R                 , "r"                 , EMPTY      , svg_attribute_r                  , NONE         )   \
DECLARE_ATTRIBUTE(RX                , "rx"                , EMPTY      , svg_attribute_rx                 , NONE         )   \
DECLARE_ATTRIBUTE(RY                , "ry"                , EMPTY      , svg_attribute_ry                 , NONE         )   \
DECLARE_ATTRIBUTE(CX                , "cx"                , EMPTY      , svg_attribute_cx                 , NONE         )   \
DECLARE_ATTRIBUTE(CY                , "cy"                , EMPTY      , svg_attribute_cy                 , NONE         )   \

enum class svg_attribute_type
{
  UNKNOWN,
#define DECLARE_ATTRIBUTE(ENUM,NAME,NAMESPACE,CLASS,INHERIT_TYPE) ENUM,
  DECLARE_SVG_ATTRIBUTES
#undef DECLARE_ATTRIBUTE

};

enum class svg_inherit_type
{
  NONE,
  INHERIT,
  STYLE,
};

#endif // ATTRIBUTE_TYPE_H

