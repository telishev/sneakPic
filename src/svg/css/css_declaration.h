#ifndef CSS_DECLARATION_H
#define CSS_DECLARATION_H

#include <map>
#include <string>

class abstract_attribute;
class abstract_svg_item;

class css_declaration
{
  std::map<std::string, abstract_attribute *> m_attributes;
  abstract_svg_item *m_item;
public:
  css_declaration (abstract_svg_item *item);
  ~css_declaration ();

  bool parse (const char *str);
  std::string to_string () const;

  abstract_attribute *get_attribute (const std::string &str) const;

private:
  bool create_attribute (const std::string &name, const std::string &value);
};

#endif // CSS_DECLARATION_H
