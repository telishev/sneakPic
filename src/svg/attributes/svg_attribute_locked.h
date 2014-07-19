#ifndef SVG_ATTRIBUTE_LOCKED_H
#define SVG_ATTRIBUTE_LOCKED_H

#include "abstract_attribute.h"

class svg_attribute_locked : public  abstract_attribute
{
  SVG_ATTRIBUTE

  bool m_is_locked = false;
public:
  svg_attribute_locked ();
  virtual ~svg_attribute_locked ();

  virtual bool read (const char *data, bool from_css = false) override;
  virtual bool write (QString &data, bool to_css = false) const override;

  bool is_locked () const { return m_is_locked; }
  void set_locked (bool locked) { m_is_locked = locked; }
};

#endif // SVG_ATTRIBUTE_LOCKED_H
