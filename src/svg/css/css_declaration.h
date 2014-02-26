#ifndef CSS_DECLARATION_H
#define CSS_DECLARATION_H

#include <map>
#include <string>

class abstract_attribute;
class items_edit_handler_t;

class css_declaration
{
  map<string, abstract_attribute *> m_attributes;
  int m_item_id;
public:
  css_declaration (int item_id);
  ~css_declaration ();

  bool parse (const char *str);
  string to_string () const;

  void set_edit_handler (items_edit_handler_t *edit_handler);
  abstract_attribute *get_attribute (const string &str) const;

  const map<string, abstract_attribute *> &attributes () const { return m_attributes; }

private:
  bool create_attribute (const string &name, const string &value);
};

#endif // CSS_DECLARATION_H
