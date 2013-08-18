#ifndef CSS_DECLARATION_H
#define CSS_DECLARATION_H

#include <map>
#include <string>

class abstract_attribute;
class svg_document;

class css_declaration
{
  std::map<std::string, abstract_attribute *> m_attributes;
  svg_document *m_document;
  int m_item_id;
public:
  css_declaration (svg_document *document, int item_id);
  ~css_declaration ();

  bool parse (const char *str);
  std::string to_string () const;

  abstract_attribute *get_attribute (const std::string &str) const;

private:
  bool create_attribute (const std::string &name, const std::string &value);
};

#endif // CSS_DECLARATION_H
