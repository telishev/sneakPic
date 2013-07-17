#include "svg/css/css_declaration.h"

#include <memory>

#include "common/memory_deallocation.h"
#include "common/string_utils.h"
#include "common/common_utils.h"

#include "svg/attributes/abstract_attribute.h"
#include "svg/attributes/svg_attribute_factory.h"

#include "svg/svg_document.h"

#include "svg/items/abstract_svg_item.h"


css_declaration::css_declaration (abstract_svg_item *item)
{
  m_item = item;
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
      std::string prop_name, prop_value;
      CHECK (extract_chunk (':', str, prop_name));
      CHECK (extract_chunk (';', str, prop_value));

      /// ignore attribute if parsing was unsuccessful
      create_attribute (prop_name, prop_value);
    }

  return true;
}

bool css_declaration::create_attribute (const std::string &name, const std::string &value)
{
  svg_attribute_factory *factory = m_item->document ()->attribute_factory ();

  std::string real_name = from_escaped_string (name);
  std::string real_value = from_escaped_string (value);

  /// TODO: do something with namespaces
  std::unique_ptr <abstract_attribute> attribute (factory->create_attribute (m_item, QString::fromStdString (real_name), QString (), QString ()));
  if (!attribute)
    return false;

  if (!attribute->read (QString::fromStdString (real_value), true))
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

std::string css_declaration::to_string () const
{
  std::string result;
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

abstract_attribute *css_declaration::get_attribute (const std::string &str) const
{
  auto it = m_attributes.find (str);
  if (it == m_attributes.end ())
    return nullptr;

  return it->second;
}
