#include "svg/attributes/svg_attributes_enum.h"

const char *enum_to_string (gradient_units id)
{
  switch (id)
    {
    case gradient_units::USER_SPACE: return "userSpaceOnUse";
    case gradient_units::OBJECT_BOUNDING_BOX: return "objectBoundingBox";
    case gradient_units::INVALID: return "";
    }

  return "";
}

const char *enum_to_string (spread_method id)
{
  switch (id)
    {
    case spread_method::PAD: return "pad";
    case spread_method::REFLECT: return "reflect";
    case spread_method::REPEAT: return "repeat";
    case spread_method::INVALID: return "";
    }

  return "";
}

const char *enum_to_string (fill_rule id)
{
  switch (id)
    {
    case fill_rule::NON_ZERO: return "nonzero";
    case fill_rule::EVEN_ODD: return "evenodd";
    case fill_rule::INVALID: return "";
    }

  return "";
}

const char *enum_to_string (visibility id)
{
  switch (id)
    {
    case visibility::VISIBLE: return "visible";
    case visibility::HIDDEN: return "hidden";
    case visibility::COLLAPSE: return "collapse";
    case visibility::INVALID: return "";
    }

  return "";
}

const char *enum_to_string (display id)
{
  switch (id)
    {
      case display::INLINE:return "inline";
      case display::BLOCK:return "block";
      case display::LIST_ITEM:return "list-item";
      case display::RUN_IN:return "run-in";
      case display::COMPACT:return "compact";
      case display::MARKER:return "marker";
      case display::TABLE:return "table";
      case display::INLINE_TABLE:return "inline-table";
      case display::TABLE_ROW_GROUP:return "table-row-group";
      case display::TABLE_HEADER_GROUP:return "table-header-group";
      case display::TABLE_FOOTER_GROUP:return "table-footer-group";
      case display::TABLE_ROW:return "table-row";
      case display::TABLE_COLUMN_GROUP:return "table-column-group";
      case display::TABLE_COLUMN:return "table-column";
      case display::TABLE_CELL:return "table-cell";
      case display::TABLE_CAPTION:return "table-caption";
      case display::NONE:return "none";
      case display::INVALID:return "";
    }

  return "";
}

const char *enum_to_string (text_anchor id)
{
  switch (id)
    {
      case text_anchor::START: return "start";
      case text_anchor::MIDDLE:return "middle";
      case text_anchor::END:   return "end";
      case text_anchor::INVALID:return "";
    }

  return "";
}


const char *enum_to_string (marker_units id)
{
  switch (id)
    {
    case marker_units::STROKE_WIDTH: return "strokeWidth";
    case marker_units::USER_SPACE: return "userSpaceOnUse";
    case marker_units::INVALID: return "";
    }

  return "";
}