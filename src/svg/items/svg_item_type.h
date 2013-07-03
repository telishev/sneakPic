#ifndef SVG_ITEM_TYPE_H
#define SVG_ITEM_TYPE_H

#define DECLARE_SVG_ITEM                                          \
/*                                                            */  \
/*            ENUM           |   NAME           |  NAMESPACE  |         CLASS            */  \
DECLARE_ITEM(SVG             , "svg"            , SVG         , svg_item_svg             )   \
DECLARE_ITEM(G               , "g"              , SVG         , svg_item_group           )   \
DECLARE_ITEM(PATH            , "path"           , SVG         , svg_item_path            )   \
DECLARE_ITEM(STYLE           , "style"          , SVG         , svg_item_style           )   \
DECLARE_ITEM(DEFS            , "defs"           , SVG         , svg_item_defs            )   \
DECLARE_ITEM(LINE            , "line"           , SVG         , svg_item_line            )   \
DECLARE_ITEM(RECT            , "rect"           , SVG         , svg_item_rect            )   \
DECLARE_ITEM(ELLIPSE         , "ellipse"        , SVG         , svg_item_ellipse         )   \
DECLARE_ITEM(CIRCLE          , "circle"         , SVG         , svg_item_circle          )   \
DECLARE_ITEM(USE             , "use"            , SVG         , svg_item_use             )   \
DECLARE_ITEM(STOP            , "stop"           , SVG         , svg_item_stop            )   \
DECLARE_ITEM(LINEAR_GRADIENT , "linearGradient" , SVG         , svg_item_linear_gradient )   \
DECLARE_ITEM(RADIAL_GRADIENT , "radialGradient" , SVG         , svg_item_radial_gradient )   \


enum class svg_item_type
{
  UNKNOWN,
#define DECLARE_ITEM(ENUM,NAME,NAMESPACE,CLASS) ENUM,
  DECLARE_SVG_ITEM
#undef DECLARE_ITEM
};


#endif // SVG_ITEM_TYPE_H
