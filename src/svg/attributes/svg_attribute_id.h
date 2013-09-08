#ifndef SVG_ATTRIBUTE_ID_H
#define SVG_ATTRIBUTE_ID_H

#include "svg/attributes/abstract_attribute.h"

#include <QString>

class svg_attribute_id : public abstract_attribute
{
  SVG_ATTRIBUTE
  std::string m_id;
public:
  svg_attribute_id () {}
  virtual ~svg_attribute_id () {}

  virtual bool read (const char *data, bool /*from_css*/ = false) override { m_id = data; return true; }
  virtual bool write (QString &data, bool /*to_css*/ = false) const override { data = QString::fromStdString (m_id); return true; }

  std::string id () const { return m_id; }
  void set_id (const std::string &id) { m_id = id; } 
};

#endif // SVG_ATTRIBUTE_ID_H
