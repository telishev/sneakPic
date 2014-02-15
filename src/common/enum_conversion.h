/// \file string/enum_conversion.h
/// \brief contains functions to convert string to enums

#ifndef ENUM_CONVERSION_H
#define ENUM_CONVERSION_H


/// to use conversion functions you must implement the following function:
/// const char *enum_to_string (E id);
/// if your enum does not specify simple continuous 0,..., N - 1, N sequence but rather have some ruptures in it (like 0, 1, 4)
/// or starts not from 0 but from higher value then you should also specify:
/// int enum_values_count (E);
/// which should return maximum possible value of enum plus 1
/// negative values in enums, for now unsupported

#include "common/memory_deallocation.h"
#include "common/string_utils.h"

#include <unordered_map>
#include <string.h>

inline int enum_values_count (int)
{
  return -1;
}

template <typename T>
int enum_values_count_template (int x = T ())
{
  return enum_values_count (T ());
}

template <typename T>
int enum_values_count_template (...)
{
  return -1;
}

template <typename E>
class enum_helper
{
  std::unordered_map<std::string, E> enum_map;
  int count;
public:
  enum_helper ()
  {
    count = enum_values_count_template<E> (E ());
    int i = 0;
    while ((count < 0) || (i < count))
      {
        const char *enum_value = enum_to_string ((E)i);
        if (*enum_value)
          enum_map.insert (std::make_pair (std::string (enum_value), (E)i));
        else if (count < 0)
          {
            count = i;
            break;
          }
        i++;
      }
  }

  int get_count ()
  {
    return count;
  }

  E get_enum_value (const char *string)
  {
    typename std::unordered_map<std::string, E>::const_iterator it = enum_map.find (std::string (string));
    if (it != enum_map.end ())
      return (*it).second;
    else
      return (E) count;
  }

  static enum_helper self;
};

template <typename E>
enum_helper<E> enum_helper<E>::self;

template <typename E>
static E string_to_enum (const char *string)
{
  return enum_helper<E>::self.get_enum_value (string);
}

template <typename E>
static int enum_count ()
{
  return enum_helper<E>::self.get_count ();
}

template <typename E>
static E process_string_fixed_length_to_enum (const char *&string, int length)
{
  char *string_in_question = 0;
  string_in_question = new char[length + 1];
  strncpy (string_in_question, string, length);
  string_in_question[length] = '\0';
  const char *trimmed_string_in_question = string_in_question;

  E res = enum_helper<E>::self.get_enum_value (trimmed_string_in_question);
  FREE_ARRAY (string_in_question);
  string += length;
  return res;
}

template <typename E>
static E process_string_before_delimiter_to_enum (const char *&string, const char delimiter = ',')
{
  const char *next_delimiter_pos = strchr (string, delimiter);
  if (!next_delimiter_pos)
    next_delimiter_pos = string + strlen (string);
  trim_whitespace_left (string);
  const char *last_pos = trim_whitespace_right (string, string + strlen (string));
  int len = last_pos - string;

  E res = process_string_fixed_length_to_enum<E> (string, len);
  if (*next_delimiter_pos)
    next_delimiter_pos++;

  string = next_delimiter_pos;
  return res;
}

template <typename E>
static E string_to_enum (const std::string &str)
{
  return string_to_enum<E> (str.c_str ());
}

#endif // ENUM_CONVERSION_H
