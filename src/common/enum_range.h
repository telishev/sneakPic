#ifndef ENUM_RANGE_H
#define ENUM_RANGE_H

#include <iterator>

#include "enum_conversion.h"

template<typename T>
class enum_range_t;

template<typename T>
class enum_range_iterator : public std::iterator<std::forward_iterator_tag, T>
{
  T m_value;
public:
  T operator *() const { return m_value; }
  enum_range_iterator<T> operator++ () { enum_range_iterator<T> val (m_value);  m_value = (T) ((int)m_value + 1); return val; }

  bool operator == (const enum_range_iterator<T> &rhs) const { return m_value == rhs.m_value; }
  bool operator != (const enum_range_iterator<T> &rhs) const { return m_value != rhs.m_value; }

private:
  enum_range_iterator (T value) {m_value = value; }
  friend class enum_range_t<T>;
};


template<typename T>
class enum_range_t
{
public:
  enum_range_iterator<T> begin () const { return enum_range_iterator<T> ((T)0); }
  enum_range_iterator<T> end () const { return enum_range_iterator<T> ((T)enum_count<T> ()); }
};

template<typename T>
enum_range_t<T> enum_range () { return enum_range_t<T> (); }

#endif // ENUM_RANGE_H
