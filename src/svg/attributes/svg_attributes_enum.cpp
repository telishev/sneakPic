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




