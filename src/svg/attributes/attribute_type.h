#ifndef ATTRIBUTE_TYPE_H
#define ATTRIBUTE_TYPE_H

#define DECLARE_SVG_ATTRIBUTES                                                                                 \
/*                                                                                                         */  \
/*                ENUM         |   NAME         |  NAMESPACE |         CLASS                | INHERIT_TYPE */  \
DECLARE_ATTRIBUTE(ID           , "id"           , EMPTY      , svg_attribute_id             , NONE         )   \
DECLARE_ATTRIBUTE(VERSION      , "version"      , EMPTY      , svg_attribute_version        , NONE         )   \
DECLARE_ATTRIBUTE(WIDTH        , "width"        , EMPTY      , svg_attribute_width          , NONE         )   \
DECLARE_ATTRIBUTE(HEIGHT       , "height"       , EMPTY      , svg_attribute_height         , NONE         )   \
DECLARE_ATTRIBUTE(D            , "d"            , EMPTY      , svg_attribute_path_data      , NONE         )   \
DECLARE_ATTRIBUTE(STYLE        , "style"        , EMPTY      , svg_attribute_style          , NONE         )   \
DECLARE_ATTRIBUTE(CLASS        , "class"        , EMPTY      , svg_attribute_class          , NONE         )   \
DECLARE_ATTRIBUTE(FILL         , "fill"         , EMPTY      , svg_attribute_fill           , STYLE        )   \
DECLARE_ATTRIBUTE(STROKE       , "stroke"       , EMPTY      , svg_attribute_stroke         , STYLE        )   \
DECLARE_ATTRIBUTE(STROKE_WIDTH , "stroke-width" , EMPTY      , svg_attribute_stroke_width   , STYLE        )   \
DECLARE_ATTRIBUTE(TRANSFORM    , "transform"    , EMPTY      , svg_attribute_transform      , INHERIT      )   \


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
