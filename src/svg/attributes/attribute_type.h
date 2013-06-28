#ifndef ATTRIBUTE_TYPE_H
#define ATTRIBUTE_TYPE_H

#define DECLARE_SVG_ATTRIBUTES                                                \
/*                                                                        */  \
/*                ENUM   |   NAME  |  NAMESPACE  |         CLASS          */  \
DECLARE_ATTRIBUTE(ID     , "id"     , EMPTY      , svg_attribute_id       )   \
DECLARE_ATTRIBUTE(VERSION, "version", EMPTY      , svg_attribute_version  )   \
DECLARE_ATTRIBUTE(WIDTH  , "width"  , EMPTY      , svg_attribute_width    )   \
DECLARE_ATTRIBUTE(HEIGHT , "height" , EMPTY      , svg_attribute_height   )   \
DECLARE_ATTRIBUTE(D      , "d"      , EMPTY      , svg_attribute_path_data)   \
DECLARE_ATTRIBUTE(STYLE  , "style"  , EMPTY      , svg_attribute_style    )   \
DECLARE_ATTRIBUTE(CLASS  , "class"  , EMPTY      , svg_attribute_class    )   \


enum class svg_attribute_type
{
  UNKNOWN,
#define DECLARE_ATTRIBUTE(ENUM,NAME,NAMESPACE,CLASS) ENUM,
  DECLARE_SVG_ATTRIBUTES
#undef DECLARE_ATTRIBUTE

};
#endif // ATTRIBUTE_TYPE_H
