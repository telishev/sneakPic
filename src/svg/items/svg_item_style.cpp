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
#include "svg_character_data.h"


svg_item_style::svg_item_style (svg_document *document)
  : abstract_svg_item (document)
{
  document->selectors ()->add_style (this);
}

svg_item_style::~svg_item_style ()
{
  document ()->selectors ()->remove_style (this);
  for (auto &rule : m_rule_set)
    {
      FREE (rule.first);
      FREE (rule.second);
    }
}

bool svg_item_style::read_item (const char *data)
{
  CHECK (skip_comments_and_whitespaces (data));
  while (*data)
    {
      std::string selectors_string, declaration_string;
      CHECK (extract_chunk ('{', data, selectors_string));
      CHECK (extract_chunk ('}', data, declaration_string));

      std::unique_ptr <css_declaration> declaration (new css_declaration (this));
      std::unique_ptr <abstract_css_selector> selector (css_selector_reader::create_selector (selectors_string.c_str ()));
      CHECK (declaration->parse (declaration_string.c_str ()));
      if (!selector)
        return false;

      m_rule_set.push_back (std::make_pair (selector.release (), declaration.release ()));
    }

  return true;
}

const abstract_attribute *svg_item_style::get_attribute (const std::string &str, const abstract_svg_item *item_to_match) const
{
  for (auto rule : m_rule_set)
    {
      abstract_css_selector *selector = rule.first;
      css_declaration *declaration = rule.second;
      const abstract_attribute *attribute = declaration->get_attribute (str);
      if (!attribute)
        continue;

      /// TODO: in fact, we need to assemble all matching selectors,
      /// choose one with the most specificity or importance etc, but its too cumbersome to do right now
      if (selector->is_matched (item_to_match))
        return attribute;
    }

  return nullptr;
}

void svg_item_style::item_read_complete ()
{
  for (const abstract_svg_item *child = first_child (); child; child = child->next_sibling ())
    {
      if (!child->is_character_data ())
        continue;

      const svg_character_data *data = static_cast<const svg_character_data *> (child);
      read_item (data->char_data ());
    }
}

