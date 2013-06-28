#include "svg/css/css_selector_reader.h"

#include <memory>

#include "common/string_utils.h"

#include "svg/css/simple_selector.h"
#include "svg/css/descendant_selector.h"
#include "svg/css/adjancent_selector.h"
#include "svg/css/child_selector.h"

static char get_combinator (const char *&str)
{
  char combinator = '\0';
  while (*str == ' ' || *str == '>' || *str == '+')
    {
      if (*str != ' ' || combinator == '\0')
        combinator = *str;

      str++;
    }

  return combinator;
}

abstract_css_selector *css_selector_reader::create_selector (const char *str)
{
  trim_whitespace_left (str);
  std::unique_ptr <simple_selector> selector (new simple_selector);
  if (!selector->create_from_string (str))
    return nullptr;

  std::unique_ptr <abstract_css_selector> result (selector.release ());

  while (*str)
    {
      char combinator = get_combinator (str);
      if (*str == '\0')
        return selector.release ();

      std::unique_ptr <simple_selector> new_selector (new simple_selector);
      if (!new_selector->create_from_string (str))
        return nullptr;

      switch (combinator)
        {
        case ' ':
          result.reset (new descendant_selector (result.release (), new_selector.release ()));
          break;
        case '+':
          result.reset (new adjancent_selector (result.release (), new_selector.release ()));
          break;
        case '>':
          result.reset (new child_selector (result.release (), new_selector.release ()));
          break;
        default:
          return nullptr;
        }
    }

  return result.release ();
}
