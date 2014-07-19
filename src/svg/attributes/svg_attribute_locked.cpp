#include "svg_attribute_locked.h"

svg_attribute_locked::svg_attribute_locked ()
{

}

svg_attribute_locked::~svg_attribute_locked ()
{

}

bool svg_attribute_locked::read (const char *data, bool /*from_css*/)
{
  if (data == "locked")
    m_is_locked = true;
  else
    m_is_locked = false;

  return true;
}

bool svg_attribute_locked::write (QString &data, bool /*to_css*/) const
{
  data = m_is_locked ? "locked" : "unlocked";
  return true;
}
