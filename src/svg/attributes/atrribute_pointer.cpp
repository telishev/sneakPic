#include "svg/attributes/atrribute_pointer.h"

#include "common/memory_deallocation.h"

#include "svg/attributes/svg_attribute_factory.h"
#include "svg/attributes/abstract_attribute.h"

#include "svg/items/abstract_svg_item.h"

#include "svg/svg_document.h"


abstract_attribute_pointer::abstract_attribute_pointer (abstract_attribute *attribute, abstract_svg_item *item)
{
  m_item = item;
  m_cur_attribute = attribute;
}

abstract_attribute_pointer::abstract_attribute_pointer (abstract_attribute_pointer &&rhs)
{
  m_cur_attribute = rhs.m_cur_attribute;
  rhs.m_cur_attribute = nullptr;
  m_item = rhs.m_item;
  rhs.m_item = nullptr;
}

abstract_attribute_pointer::~abstract_attribute_pointer ()
{
  if (!m_item)
    return;

  m_item->signal_attribute_change_end (m_cur_attribute);
}
