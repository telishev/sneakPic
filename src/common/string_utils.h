#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <string>

class QColor;
class QString;

int is_separator (const unsigned char c);

void trim_separators_left (const char *&str);

int is_space (const unsigned char c);

void trim_whitespace_left (const char *&str);

/// returns last non-whitespace character before str_end
const char *trim_whitespace_right (const char *str_begin, const char *str_end);

/// checks if target_str start with possible prefix
bool starts_with (const char *target_str, const char *possible_prefix);

/// checks if target_str start with possible prefix and if it is shifts target_str to position after that prefix
bool starts_with_and_shift (const char *&target_str, const char *possible_prefix);

/// Analog of isalpha function from ctype.h (for default locale)
int is_alpha (const unsigned char c);

/// Analog of isalnum function from ctype.h (for default locale)
int is_num (const unsigned char c);

/// Analog of isalnum function from ctype.h (for default locale)
int is_alnum (const unsigned char c);

/// returns next identifier (string consisting of numbers and letters)
/// moves str to the end of the identifier
std::string get_next_identifier (const char *&str);


///  finds end of comment
bool goto_comment_end (const char *&str);

/// skips comments and whitespaces, sets str to the first non-space and comment symbol or to string end
bool skip_comments_and_whitespaces (const char *&str);

/// find a char or end of string in string, considering escape chars, strings etc
/// sets str to that symbol's address or to string end
bool goto_next_char (const char *&str, char to_find);

/// returns string where escape characters are replaced
std::string from_escaped_string (const std::string &str);

/// returns string with special symbols escaped
std::string to_escaped_string (const std::string &str);

bool extract_chunk (char chunk_end, const char *&data, std::string &result);

/**
  \brief convert string to double value, e.g 1.32E+3, -32D-4
  \return strtod return value
 */
double str_to_double (
  const char *string,   /* A decimal ASCII floating-point number,
                         * optionally preceded by white space.
                         * Must have form "-I.FE-X", where I is the
                         * integer part of the mantissa, F is the
                         * fractional part of the mantissa, and X
                         * is the exponent.  Either of the signs
                         * may be "+", "-", or omitted.  Either I
                         * or F may be omitted, or both.  The decimal
                         * point isn't necessary unless F is present.
                          *  E and X may both be omitted (but not just one).
                         */
  const char **endPtr   /* If non-0, store terminating character's
                         * address here. */
  );

/// parses double from a string
/// string pointer is moved to an end of the parsed double
/// returns if parsing was successful
bool str_to_double (const char *&string, double &data);

QString hex_to_str (int value);

// transform color to typical svg string format
QString color_to_string (const QColor &color);

// read everything which can be put into color or fill, stroke attributes(except currentColor) and returns resulting QColor
QColor string_to_color (const char *string);

QString double_to_str (double value);

#endif // STRING_UTILS_H
