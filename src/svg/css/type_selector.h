#ifndef TYPE_SELECTOR_H
#define TYPE_SELECTOR_H

#include "svg/css/abstract_css_selector.h"

class type_selector : public abstract_css_selector
{
  string m_type;
public:
  type_selector (const string &type);
  ~type_selector ();

  virtual bool is_matched (const abstract_svg_item *item) const override;
  virtual string to_string () const override { return m_type; }

  virtual bool can_be_first () const { return true; }
};


#endif // TYPE_SELECTOR_H
