#include "svg/css/css_declaration.h"

#include <memory>
#include <QString>

#include "common/memory_deallocation.h"
#include "common/string_utils.h"
#include "common/common_utils.h"

#include "svg/attributes/abstract_attribute.h"
#include "svg/attributes/svg_attribute_factory.h"

#include "svg/items/svg_item_type.h"


css_declaration::css_declaration (int item_id)
{
  m_item_id = item_id;
}

css_declaration::~css_declaration ()
{
  for (auto &attribute_pair : m_attributes)
    {
      FREE (attribute_pair.second);
    }
}

bool css_declaration::parse (const char *str)
{
  CHECK (skip_comments_and_whitespaces (str));

  while (*str)
    {
      string prop_name, prop_value;
      CHECK (extract_chunk (':', str, prop_name));
      CHECK (extract_chunk (';', str, prop_value));

      /// ignore attribute if parsing was unsuccessful
      create_attribute (prop_name, prop_value);
    }

  return true;
}

bool css_declaration::create_attribute (const string &name, const string &value)
{
  string real_name = from_escaped_string (name);
  string real_value = from_escaped_string (value);

  /// TODO: do something with namespaces
  unique_ptr <abstract_attribute> attribute (svg_attribute_factory::get ()->create_attribute (real_name.c_str (), svg_item_type::STYLE));
  if (!attribute)
    return false;

  attribute->set_item (m_item_id);
  if (!attribute->read (real_value.c_str (), true))
    return false;

  auto it = m_attributes.find (name);
  if (it != m_attributes.end ())
    {
      FREE (it->second);
      m_attributes.erase (it);
    }

  m_attributes.insert (std::make_pair (name, attribute.release ()));
  return true;
}

string css_declaration::to_string () const
{
  string result;
  bool first = true;
  for (auto &attribute_pair : m_attributes)
    {
      const abstract_attribute *attribute = attribute_pair.second;
      if (!first)
        result.push_back (';');

      first = false;
      QString value;
      attribute->write (value, true);
      result += QString (QString (attribute->type_name ()) + ":" + value).toStdString ();
    }

  return result;
}

abstract_attribute *css_declaration::get_attribute (const string &str) const
{
  auto it = m_attributes.find (str);
  if (it == m_attributes.end ())
    return nullptr;

  return it->second;
}

void css_declaration::set_edit_handler (items_edit_handler_t *edit_handler)
{
  for (auto &attribute_pair : m_attributes)
    {
      attribute_pair.second->set_edit_handler (edit_handler);
    }
}
