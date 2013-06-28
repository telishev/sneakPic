#ifndef UNIVERSAL_SELECTOR_H
#define UNIVERSAL_SELECTOR_H

#include "svg/css/abstract_css_selector.h"

class universal_selector : public abstract_css_selector
{
public:
  universal_selector () {}
  ~universal_selector () {}

  virtual bool is_matched (const abstract_svg_item * /*item*/) const override { return true; }
  virtual std::string to_string () const override { return "*"; }
  virtual bool can_be_first () const override{ return true; }
};

#endif // UNIVERSAL_SELECTOR_H
