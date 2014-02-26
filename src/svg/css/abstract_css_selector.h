#ifndef ABSTRACT_CSS_SELECTOR_H
#define ABSTRACT_CSS_SELECTOR_H

#include <string>

class abstract_svg_item;

class abstract_css_selector
{
public:
  virtual ~abstract_css_selector () {}

  /// returns if item is selected by this selector
  virtual bool is_matched (const abstract_svg_item *item) const = 0;

  /// converts selector to string
  virtual string to_string () const = 0;

  /// whether it can be first selector in simple selector
  virtual bool can_be_first () const = 0;
};


#endif // ABSTRACT_CSS_SELECTOR_H
