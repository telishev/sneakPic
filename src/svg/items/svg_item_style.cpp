#include "svg/items/svg_item_style.h"

#include <memory>

#include "common/string_utils.h"
#include "common/common_utils.h"
#include "common/memory_deallocation.h"

#include "svg/css/css_declaration.h"
#include "svg/css/css_selector_reader.h"
#include "svg/css/abstract_css_selector.h"
#include "svg/css/selectors_container.h"

#include "svg/svg_document.h"
#include "svg/items/svg_character_data.h"


svg_item_style::svg_item_style (svg_document *document)
  : abstract_svg_item (document)
{
}

svg_item_style::~svg_item_style ()
{
}

void svg_item_style::add_style_to_container (selectors_container *container)
{
  for (int i = 0; i < children_count (); i++)
    {
      if (!child (i)->is_character_data ())
        continue;

      const svg_character_data *data = static_cast<const svg_character_data *> (child (i));
      read_item (data->char_data (), container);
    }
}

bool svg_item_style::read_item (const char *data, selectors_container *container)
{
  CHECK (skip_comments_and_whitespaces (data));
  while (*data)
    {
      std::string selectors_string, declaration_string;
      CHECK (extract_chunk ('{', data, selectors_string));
      CHECK (extract_chunk ('}', data, declaration_string));

      unique_ptr <css_declaration> declaration (new css_declaration (undo_id ()));
      unique_ptr <abstract_css_selector> selector (css_selector_reader::create_selector (selectors_string.c_str ()));
      CHECK (declaration->parse (declaration_string.c_str ()));
      if (!selector)
        return false;

      container->add_rule_set (selector.release (), declaration.release ());
    }

  return true;
}


