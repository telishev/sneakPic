/// \file common/enum_conversion.h
/// \brief contains functions to convert string to enums

#ifndef ENUM_CONVERSION_H
#define ENUM_CONVERSION_H


/// to use conversion functions you must implement the following functions:
/// const char *enum_to_string (E id);
/// int enum_values_count (E);

#include <string>

template <typename E>
E string_to_enum (const char *string)
{
  for (int i = 0; i < enum_values_count (E ()); i++)
    if (!strcmp (string, enum_to_string ((E)i)))
      return (E)i;

  return (E)enum_values_count (E ());
}

template <typename E>
E string_to_enum (const std::string &str)
{
  return string_to_enum<E> (str.c_str ());
}

#endif // ENUM_CONVERSION_H
