#ifndef RANGE_ALGORITHM_H
#define RANGE_ALGORITHM_H

namespace range {

template<typename Container, typename T>
void erase (Container &container, const T &value)
{
  container.erase (std::remove (container.begin (), container.end (), value), container.end ());
}

template<typename Container, typename UnaryPredicate>
void erase_if (Container &container, UnaryPredicate pred)
{
  container.erase (std::remove_if (container.begin (), container.end (), std::move (pred)), container.end ());
}

template <typename Range, typename UnaryPredicate>
bool all_of (Range &&range, UnaryPredicate pred)
{
  return std::all_of (range.begin (), range.end (), std::move (pred));
}

template <typename Range, typename UnaryPredicate>
bool any_of (Range &&range, UnaryPredicate pred)
{
  return std::any_of (range.begin (), range.end (), std::move (pred));
}

template <typename Range, typename UnaryPredicate>
bool none_of (Range &&range, UnaryPredicate pred)
{
  return std::any_of (range.begin (), range.end (), std::move (pred));
}

template<typename Range, typename Function>
Function for_each (Range &&range, Function fn)
{
  return std::for_each (range.begin (), range.end (), std::move (fn));
}

template<typename Range, typename T>
auto find (Range &&range, const T& val) -> decltype (range.begin ())
{
  return std::find (range.begin (), range.end (), val);
}

template<typename Range, typename UnaryPredicate>
auto find_if (Range &&range, UnaryPredicate pred) -> decltype (range.begin ())
{
  return std::find_if (range.begin (), range.end (), std::move (pred));
}

template<typename Range, typename UnaryPredicate>
auto find_if_not (Range &&range, UnaryPredicate pred) -> decltype (range.begin ())
{
  return std::find_if_not (range.begin (), range.end (), std::move (pred));
}

template <typename Range1, typename Range2>
auto find_end (Range1 &&range1, Range2 &&range2) -> decltype (range1.begin ())
{
  return std::find_end (range1.begin (), range1.end (), range2.begin (), range2.end ());
}

template <typename Range1, typename Range2, typename BinaryPredicate>
auto find_end (Range1 &&range1, Range2 &&range2, BinaryPredicate pred) -> decltype (range1.begin ())
{
  return std::find_end (range1.begin (), range1.end (), range2.begin (), range2.end (), std::move (pred));
}

template <typename Range1, typename Range2>
auto find_first_of (Range1 &&range1, Range2 &&range2) -> decltype (range1.begin ())
{
  return std::find_first_of (range1.begin (), range1.end (), range2.begin (), range2.end ());
}

template <typename Range1, typename Range2, typename BinaryPredicate>
auto find_first_of (Range1 &&range1, Range2 &&range2, BinaryPredicate pred) -> decltype (range1.begin ())
{
  return std::find_first_of (range1.begin (), range1.end (), range2.begin (), range2.end (), std::move (pred));
}

template<typename Range>
auto adjacent_find (Range &&range) -> decltype (range.begin ())
{
  return std::adjacent_find (range.begin (), range.end ());
}

template<typename Range, typename BinaryPredicate>
auto adjacent_find (Range &&range, BinaryPredicate pred) -> decltype (range.begin ())
{
  return std::adjacent_find (range.begin (), range.end (), std::move (pred));
}

template<typename Range, typename T>
auto count (Range &&range, const T &val) -> decltype (std::count (range.begin (), range.end (), val))
{
  return std::count (range.begin (), range.end (), val);
}

template<typename Range, typename UnaryPredicate>
auto count (Range &&range, UnaryPredicate val) -> decltype (std::count (range.begin (), range.end (), val))
{
  return std::count (range.begin (), range.end (), std::move (val));
}

template <typename Range1, typename Range2>
auto mismatch (Range1 &&range1, Range2 &&range2) -> decltype (std::mismatch (range1.begin (), range1.end (), range2.begin ()))
{
  return std::mismatch (range1.begin (), range1.end (), range2.begin ());
}

template <typename Range1, typename Range2, typename BinaryPredicate>
auto mismatch (Range1 &&range1, Range2 &&range2, BinaryPredicate pred) -> decltype (std::mismatch (range1.begin (), range1.end (), range2.begin (), pred))
{
  return std::mismatch (range1.begin (), range1.end (), range2.begin (), std::move (pred));
}

template <typename Range1, typename Range2>
bool equal (Range1 &&range1, Range2 &&range2)
{
  return std::equal (range1.begin (), range1.end (), range2.begin ());
}

template <typename Range1, typename Range2, typename BinaryPredicate>
bool equal (Range1 &&range1, Range2 &&range2, BinaryPredicate pred)
{
  return std::equal (range1.begin (), range1.end (), range2.begin (), std::move (pred));
}

template <typename Range1, typename Range2>
bool is_permutation (Range1 &&range1, Range2 &&range2)
{
  return std::is_permutation (range1.begin (), range1.end (), range2.begin ());
}

template <typename Range1, typename Range2, typename BinaryPredicate>
bool is_permutation (Range1 &&range1, Range2 &&range2, BinaryPredicate pred)
{
  return std::is_permutation (range1.begin (), range1.end (), range2.begin (), std::move (pred));
}

template <typename Range1, typename Range2>
auto search (Range1 &&range1, Range2 &&range2) -> decltype (range1.begin ())
{
  return std::search (range1.begin (), range1.end (), range2.begin (), range2.end ());
}

template <typename Range1, typename Range2, typename BinaryPredicate>
auto search (Range1 &&range1, Range2 &&range2, BinaryPredicate pred) -> decltype (range1.begin ())
{
  return std::search (range1.begin (), range1.end (), range2.begin (), range2.end (), std::move (pred));
}

template <typename Range, typename Size, typename T>
auto search_n (Range &&range, Size count, const T& val) -> decltype (range.begin ())
{
  return std::search_n (range.begin (), range.end (), std::move (count), val);
}

template <typename Range, typename Size, typename T, typename BinaryPredicate>
auto search_n (Range &&range, Size count, const T& val, BinaryPredicate pred) -> decltype (range.begin ())
{
  return std::search_n (range.begin (), range.end (), std::move (count), val, std::move (pred));
}

template <typename RangeIn, typename RangeOut>
auto copy (RangeIn &&range_in, RangeOut &&range_out) -> decltype (range_out.begin ())
{
  return std::copy (range_in.begin (), range_in.end (), range_out.begin ());
}

template <typename RangeIn, typename RangeOut, typename UnaryPredicate>
auto copy_if (RangeIn &&range_in, RangeOut &&range_out, UnaryPredicate pred) -> decltype (range_out.begin ())
{
  return std::copy_if (range_in.begin (), range_in.end (), range_out.begin (), pred);
}

template <typename RangeIn, typename RangeOut>
auto copy_backward (RangeIn &&range_in, RangeOut &&range_out) -> decltype (range_out.begin ())
{
  return std::copy_backward (range_in.begin (), range_in.end (), range_out.end ());
}

template <typename RangeIn, typename RangeOut>
auto move (RangeIn &&range_in, RangeOut &&range_out) -> decltype (range_out.begin ())
{
  return std::move (range_in.begin (), range_in.end (), range_out.begin ());
}

template <typename RangeIn, typename RangeOut>
auto move_backward (RangeIn &&range_in, RangeOut &&range_out) -> decltype (range_out.begin ())
{
  return std::move_backward (range_in.begin (), range_in.end (), range_out.end ());
}

template <typename RangeIn, typename RangeOut>
auto swap_ranges (RangeIn &&range_in, RangeOut &&range_out) -> decltype (range_out.begin ())
{
  return std::swap_ranges (range_in.begin (), range_in.end (), range_out.begin ());
}

template <typename RangeIn, typename RangeOut, typename UnaryOperation>
auto transform (RangeIn &&range_in, RangeOut &&range_out, UnaryOperation op) -> decltype (range_out.begin ())
{
  return std::transform (range_in.begin (), range_in.end (), range_out.begin (), op);
}

template <typename RangeIn1, typename RangeIn2, typename RangeOut, typename BinaryOperation>
auto transform (RangeIn1 &&range_in1, RangeIn2 &&range_in2, RangeOut &&range_out, BinaryOperation op) -> decltype (range_out.begin ())
{
  return std::transform (range_in1.begin (), range_in1.end (), range_in2.begin (), range_out.begin (), op);
}

template <typename Range, typename T>
void replace (Range &&range, const T &old_value, const T &new_value)
{
  return std::replace (range.begin (), range.end (), old_value, new_value);
}

template <typename Range, typename UnaryPredicate, typename T>
void replace_if (Range &&range, UnaryPredicate pred, const T &new_value)
{
  return std::replace_if (range.begin (), range.end (), pred, new_value);
}

template <typename RangeIn, typename RangeOut, typename T>
void replace_copy (RangeIn &&range_in, RangeOut &&range_out, const T &old_value, const T &new_value)
{
  return std::replace_copy (range_in.begin (), range_in.end (), range_out.begin (), old_value, new_value);
}

template <typename RangeIn, typename RangeOut, typename UnaryPredicate, typename T>
void replace_copy_if (RangeIn &&range_in, RangeOut &&range_out, UnaryPredicate pred, const T &new_value)
{
  return std::replace_copy_if (range_in.begin (), range_in.end (), range_out.begin (), pred, new_value);
}

template <typename Range, class T>
void fill (Range &&range, const T& val)
{
  return std::fill (range.begin (), range.end (), val);
}

template <typename Range, class Generator>
void generate (Range &&range, Generator gen)
{
  return std::generate (range.begin (), range.end (), gen);
}

template <typename Range, class T>
void remove (Range &&range, const T &val)
{
  return std::remove (range.begin (), range.end (), val);
}

template <typename Range, class UnaryPredicate>
void remove_if (Range &&range, UnaryPredicate pred)
{
  return std::remove_if (range.begin (), range.end (), pred);
}

template <typename RangeIn, typename RangeOut, class T>
void remove_copy (RangeIn &&range_in, RangeOut &&range_out, const T &val)
{
  return std::remove_copy (range_in.begin (), range_in.end (), range_out.out (), val);
}

template <typename RangeIn, typename RangeOut, typename UnaryPredicate>
auto remove_copy (RangeIn &&range_in, RangeOut &&range_out, UnaryPredicate pred) -> decltype (range_out.begin ())
{
  return std::remove_copy_if (range_in.begin (), range_in.end (), range_out.out (), pred);
}

template <typename Range>
auto unique (Range &&range) -> decltype (range.begin ())
{
  return std::unique (range.begin (), range.end ());
}

template <typename Range, class BinaryPredicate>
auto unique (Range &&range, BinaryPredicate pred) -> decltype (range.begin ())
{
  return std::unique (range.begin (), range.end (), pred);
}

template <typename Range>
void reverse (Range &&range)
{
  return std::reverse (range.begin (), range.end ());
}

template <typename RangeIn, typename RangeOut>
auto reverse_copy (RangeIn &&range_in, RangeOut &&range_out) -> decltype (range_out.begin ())
{
  return std::reverse_copy (range_in.begin (), range_in.end (), range_out.begin ());
}

template <typename Range, class UnaryPredicate>
bool is_partitioned (Range &&range, UnaryPredicate pred)
{
  return std::is_partitioned (range.begin (), range.end (), pred);
}

template <typename Range, class UnaryPredicate>
auto partition (Range &&range, UnaryPredicate pred) -> decltype (range.begin ())
{
  return std::partition (range.begin (), range.end (), pred);
}

template <typename Range, class UnaryPredicate>
auto stable_partition (Range &&range, UnaryPredicate pred) -> decltype (range.begin ())
{
  return std::stable_partition (range.begin (), range.end (), pred);
}

template <typename Range>
void sort (Range &&range)
{
  return std::sort (range.begin (), range.end ());
}

template <typename Range, typename Compare>
void sort (Range &&range, Compare comp)
{
  return std::sort (range.begin (), range.end (), comp);
}

template <typename Range>
void stable_sort (Range &&range)
{
  return std::sort (range.begin (), range.end ());
}

template <typename Range, typename Compare>
void stable_sort (Range &&range, Compare comp)
{
  return std::sort (range.begin (), range.end (), comp);
}

template <typename Range>
auto min_element (Range &&range) -> decltype (range.begin ())
{
  return std::min_element (range.begin (), range.end ());
}

template <typename Range, typename Compare>
auto min_element (Range &&range, Compare comp) -> decltype (range.begin ())
{
  return std::min_element (range.begin (), range.end (), comp);
}

template <typename Range>
auto max_element (Range &&range) -> decltype (range.begin ())
{
  return std::min_element (range.begin (), range.end ());
}

template <typename Range, typename Compare>
auto max_element (Range &&range, Compare comp) -> decltype (range.begin ())
{
  return std::min_element (range.begin (), range.end (), comp);
}

};
#endif // RANGE_ALGORITHM_H
