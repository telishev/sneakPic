#ifndef GROUP_SELECTOR_H
#define GROUP_SELECTOR_H

#include "svg/css/abstract_css_selector.h"

#include <vector>

class simple_selector;

class group_selector : public abstract_css_selector
{
  vector<abstract_css_selector *> m_components;

public:
  group_selector ();
  virtual ~group_selector ();

  virtual bool is_matched (const abstract_svg_item *item) const override;
  virtual string to_string () const override;
  virtual bool can_be_first () const override { return false; }

  void add_selector (abstract_css_selector *selector) { m_components.push_back (selector); }
};

#endif // GROUP_SELECTOR_H
