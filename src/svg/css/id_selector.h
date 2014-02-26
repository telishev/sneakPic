#ifndef ID_SELECTOR_H
#define ID_SELECTOR_H

#include "svg/css/abstract_css_selector.h"

class id_selector : public abstract_css_selector
{
  string m_id_name;
public:
  id_selector (const string &id_name);

  virtual bool is_matched (const abstract_svg_item *item) const override;
  virtual string to_string () const override { return string ("#") + m_id_name; }
  virtual bool can_be_first () const override { return false; }
};

#endif // ID_SELECTOR_H
