#ifndef SIMPLE_SELECTOR_H
#define SIMPLE_SELECTOR_H

#include "svg/css/abstract_css_selector.h"

#include <vector>

class simple_selector : public abstract_css_selector
{
  vector<abstract_css_selector *> m_components;

public:
  simple_selector ();
  virtual ~simple_selector ();

  virtual bool is_matched (const abstract_svg_item *item) const override;
  virtual std::string to_string () const override;
  virtual bool can_be_first () const override{ return false; }

  /// parses string into components and sets string to simple selector's end
  /// returns false on parsing error
  bool create_from_string (const char *&str);

private:
  void add_selector (abstract_css_selector *selector);
};

#endif // SIMPLE_SELECTOR_H
