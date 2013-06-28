#include "svg/css/simple_selector.h"

#include "common/memory_deallocation.h"
#include "common/string_utils.h"

#include "svg/css/universal_selector.h"
#include "svg/css/type_selector.h"
#include "svg/css/class_selector.h"
#include "svg/css/id_selector.h"

simple_selector::simple_selector ()
{

}

simple_selector::~simple_selector ()
{
  for (abstract_css_selector *selector : m_components)
    {
      FREE (selector);
    }
}

bool simple_selector::is_matched (const abstract_svg_item *item) const 
{
  for (abstract_css_selector *selector : m_components)
    if (!selector->is_matched (item))
      return false;

  return true;
}

std::string simple_selector::to_string () const 
{
  std::string result;
  for (abstract_css_selector *selector : m_components)
    result += selector->to_string ();

  return result;
}

bool simple_selector::create_from_string (const char *&str)
{
 std::string first_id = get_next_identifier (str);
 if (first_id.empty ())
   {
     if (*str == '*')
       str++;

     add_selector (new universal_selector);
   }
 else
   add_selector (new type_selector (first_id));

  while (*str)
    {
      char command = *str;
      if (command == ' ')
        return true;

      str++;
      std::string cur_str = get_next_identifier (str);
      if (cur_str.empty ())
        return false;

      switch (command)
        {
        case '.':
          add_selector (new class_selector (cur_str));
          break;
        case '#':
          add_selector (new id_selector (cur_str));
          break;
        case ' ': return true;
        default:
          return false;
        }
    }

  return true;
}

void simple_selector::add_selector (abstract_css_selector *selector)
{
  m_components.push_back (selector);
}

