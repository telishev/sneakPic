#ifndef CHILD_SELECTOR_H
#define CHILD_SELECTOR_H

#include "svg/css/combinator_selector.h"

class child_selector : public combinator_selector
{
public:
  child_selector (abstract_css_selector *lhs, abstract_css_selector *rhs)
    : combinator_selector (lhs, rhs) {}

  virtual bool is_matched (const abstract_svg_item *item) const override;

protected:
  virtual std::string combinator_str () const override { return ">"; }
};

#endif // CHILD_SELECTOR_H
