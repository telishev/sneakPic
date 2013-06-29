#ifndef ATTRIBUTE_TYPE_H
#define ATTRIBUTE_TYPE_H

#define DECLARE_SVG_ATTRIBUTES                                                                                 \
/*                                                                                                         */  \
/*                ENUM         |   NAME         |  NAMESPACE |         CLASS                |   STYLABLE   */  \
DECLARE_ATTRIBUTE(ID           , "id"           , EMPTY      , svg_attribute_id             , false        )   \
DECLARE_ATTRIBUTE(VERSION      , "version"      , EMPTY      , svg_attribute_version        , false        )   \
DECLARE_ATTRIBUTE(WIDTH        , "width"        , EMPTY      , svg_attribute_width          , false        )   \
DECLARE_ATTRIBUTE(HEIGHT       , "height"       , EMPTY      , svg_attribute_height         , false        )   \
DECLARE_ATTRIBUTE(D            , "d"            , EMPTY      , svg_attribute_path_data      , false        )   \
DECLARE_ATTRIBUTE(STYLE        , "style"        , EMPTY      , svg_attribute_style          , false        )   \
DECLARE_ATTRIBUTE(CLASS        , "class"        , EMPTY      , svg_attribute_class          , false        )   \
DECLARE_ATTRIBUTE(FILL         , "fill"         , EMPTY      , svg_attribute_fill           , true         )   \
DECLARE_ATTRIBUTE(STROKE       , "stroke"       , EMPTY      , svg_attribute_stroke         , true         )   \
DECLARE_ATTRIBUTE(STROKE_WIDTH , "stroke-width" , EMPTY      , svg_attribute_stroke_width   , true         )   \


enum class svg_attribute_type
{
  UNKNOWN,
#define DECLARE_ATTRIBUTE(ENUM,NAME,NAMESPACE,CLASS,STYLABLE) ENUM,
  DECLARE_SVG_ATTRIBUTES
#undef DECLARE_ATTRIBUTE

};
#endif // ATTRIBUTE_TYPE_H
