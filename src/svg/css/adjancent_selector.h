#ifndef ADJANCENT_SELECTOR_H
#define ADJANCENT_SELECTOR_H

#include "svg/css/combinator_selector.h"

class adjancent_selector : public combinator_selector
{
public:
  adjancent_selector (abstract_css_selector *lhs, abstract_css_selector *rhs)
    : combinator_selector (lhs, rhs) {}

  virtual bool is_matched (const abstract_svg_item *item) const override;

protected:
  virtual string combinator_str () const override { return "+"; }
};


#endif // ADJANCENT_SELECTOR_H
