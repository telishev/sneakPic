#include "svg/css/css_selector_reader.h"

#include <memory>

#include "common/string_utils.h"
#include "common/common_utils.h"

#include "svg/css/simple_selector.h"
#include "svg/css/descendant_selector.h"
#include "svg/css/adjancent_selector.h"
#include "svg/css/child_selector.h"
#include "svg/css/group_selector.h"


abstract_css_selector *css_selector_reader::create_selector (const char *data)
{
  CHECK_RET (skip_comments_and_whitespaces (data), nullptr);

  unique_ptr<group_selector> group (new group_selector);

  while (*data)
    {
      string single_selector_string;
      CHECK_RET (extract_chunk (',', data, single_selector_string), nullptr);
      abstract_css_selector *selector = create_single_selector (single_selector_string.c_str ());
      if (!selector)
        return nullptr;

      group->add_selector (selector);
    }

  return group.release ();
}


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

abstract_css_selector *css_selector_reader::create_single_selector (const char *str)
{
  trim_whitespace_left (str);
  unique_ptr <simple_selector> selector (new simple_selector);
  if (!selector->create_from_string (str))
    return nullptr;

  unique_ptr <abstract_css_selector> result (selector.release ());

  while (*str)
    {
      char combinator = get_combinator (str);
      if (*str == '\0')
        return selector.release ();

      unique_ptr <simple_selector> new_selector (new simple_selector);
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

