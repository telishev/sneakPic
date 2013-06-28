#ifndef DESCENDANT_SELECTOR_H
#define DESCENDANT_SELECTOR_H

#include "svg/css/combinator_selector.h"

class descendant_selector : public combinator_selector
{
public:
  descendant_selector (abstract_css_selector *lhs, abstract_css_selector *rhs)
    : combinator_selector (lhs, rhs) {}

  virtual bool is_matched (const abstract_svg_item *item) const override;

protected:
  virtual std::string combinator_str () const override { return " "; }
};
#endif // DESCENDANT_SELECTOR_H
