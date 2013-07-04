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
