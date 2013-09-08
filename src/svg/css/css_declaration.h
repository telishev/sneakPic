#ifndef CSS_DECLARATION_H
#define CSS_DECLARATION_H

#include <map>
#include <string>

class abstract_attribute;
class items_edit_handler_t;

class css_declaration
{
  std::map<std::string, abstract_attribute *> m_attributes;
  int m_item_id;
public:
  css_declaration (int item_id);
  ~css_declaration ();

  bool parse (const char *str);
  std::string to_string () const;

  void set_edit_handler (items_edit_handler_t *edit_handler);
  abstract_attribute *get_attribute (const std::string &str) const;

  const std::map<std::string, abstract_attribute *> &attributes () const { return m_attributes; }

private:
  bool create_attribute (const std::string &name, const std::string &value);
};

#endif // CSS_DECLARATION_H
