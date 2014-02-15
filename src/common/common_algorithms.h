#ifndef COMMON_ALGORITHMS_H
#define COMMON_ALGORITHMS_H

template <typename Container>
typename Container::mapped_type value_or (Container const &container, const typename Container::key_type &key, const typename Container::mapped_type &default_value = typename Container::mapped_type ())
{
  typename Container::const_iterator it = container.find (key);
  return (it != container.end ()) ? it->second : default_value;
}

template<typename Iter>
std::pair<Iter, Iter> slide (Iter begin, Iter end, Iter target)
{
  if (target < begin) return std::make_pair (target, std::rotate (target, begin, end));
  if (end < target) return std::make_pair (std::rotate (begin, end, target), target);
  return std::make_pair (begin, end);
}
 

#endif // COMMON_ALGORITHMS_H
