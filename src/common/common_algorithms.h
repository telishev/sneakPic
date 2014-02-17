#ifndef COMMON_ALGORITHMS_H
#define COMMON_ALGORITHMS_H

template <typename Container>
typename Container::mapped_type value_or (Container const &container, const typename Container::key_type &key, const typename Container::mapped_type &default_value = typename Container::mapped_type ())
{
  typename Container::const_iterator it = container.find (key);
  return (it != container.end ()) ? it->second : default_value;
}

// GCC workaround - 4.8.2 still has old version of std::rotate (which returns void)
template<typename Iter>
Iter std_rotate (Iter first, Iter middle, Iter last)
{
  std::rotate (first, middle, last);
  std::advance (first, std::distance (middle, last));
  return first;
}

template<typename Iter>
std::pair<Iter, Iter> slide (Iter begin, Iter end, Iter target)
{
  if (target < begin) return std::make_pair (target, std_rotate (target, begin, end));
  if (end < target) return std::make_pair (std_rotate (begin, end, target), target);
  return std::make_pair (begin, end);
}


#endif // COMMON_ALGORITHMS_H
