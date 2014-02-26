#ifndef COMBINATOR_SELECTOR_H
#define COMBINATOR_SELECTOR_H

#include "svg/css/abstract_css_selector.h"

class combinator_selector : public abstract_css_selector
{
protected:
  abstract_css_selector *m_lhs;
  abstract_css_selector *m_rhs;
public:
  combinator_selector (abstract_css_selector *lhs, abstract_css_selector *rhs);
  virtual ~combinator_selector ();

  virtual string to_string () const override;
  virtual bool can_be_first () const override { return false; }

protected:
  virtual string combinator_str () const = 0;
};

#endif // COMBINATOR_SELECTOR_H
