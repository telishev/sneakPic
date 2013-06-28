#ifndef CLASS_SELECTOR_H
#define CLASS_SELECTOR_H

#include "svg/css/abstract_css_selector.h"

class class_selector : public abstract_css_selector
{
  std::string m_class_name;
public:
  class_selector (const std::string &class_name);

  virtual bool is_matched (const abstract_svg_item *item) const override;
  virtual std::string to_string () const override { return std::string (".") + m_class_name; }
  virtual bool can_be_first () const override { return false; }
};


#endif // CLASS_SELECTOR_H
