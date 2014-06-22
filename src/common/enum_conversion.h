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

template<typename T>
static inline
int enum_values_count (T , decltype(T::COUNT) = T{}) { return (int)T::COUNT; }

static inline
int enum_values_count (...) { return -1; }

template <typename E>
class enum_helper_count
{
  int count;
public:
  enum_helper_count () { count = enum_values_count (E ()); }
  int get_count () { return count; }
  static enum_helper_count self;
};

template <typename E>
enum_helper_count<E> enum_helper_count<E>::self;

template <typename E>
class enum_helper_string
{
  std::unordered_map<string, E> enum_map;
  int count;
public:

  enum_helper_string ()
  {
    count = enum_values_count (E ());
    int i = 0;
    while ((count < 0) || (i < count))
      {
        const char *enum_value = enum_to_string ((E)i);
        if (*enum_value)
          enum_map.insert (std::make_pair (string (enum_value), (E)i));
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

  E get_enum_value (const char *str)
  {
    typename std::unordered_map<string, E>::const_iterator it = enum_map.find (string (str));
    if (it != enum_map.end ())
      return (*it).second;
    else
      return (E) count;
  }

  static enum_helper_string self;
};

template <typename E>
enum_helper_string<E> enum_helper_string<E>::self;

template<typename E>
enum_helper_string<E> &get_enum_helper (E, decltype (enum_to_string (E ())) = nullptr)
{
  return enum_helper_string<E>::self;
}

template<typename E>
enum_helper_count<E> &get_enum_helper (E, ...)
{
  return enum_helper_count<E>::self;
}

template <typename E>
static E string_to_enum (const char *string)
{
  return get_enum_helper (E (), nullptr).get_enum_value (string);
}

template <typename E>
static int enum_count ()
{
  return get_enum_helper (E (), nullptr).get_count ();
}

template <typename E>
static E process_string_fixed_length_to_enum (const char *&string, int length)
{
  char *string_in_question = nullptr;
  string_in_question = new char[length + 1];
  strncpy (string_in_question, string, length);
  string_in_question[length] = '\0';
  const char *trimmed_string_in_question = string_in_question;

  E res = get_enum_helper (E (), nullptr).get_enum_value (trimmed_string_in_question);
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
static E string_to_enum (const string &str)
{
  return string_to_enum<E> (str.c_str ());
}

#endif // ENUM_CONVERSION_H
