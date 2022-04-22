#pragma once
#include <concepts>
#include <functional>
#include <iostream>
#include <vector>

namespace experimental {
template <class T> concept container = requires(T a, const T b) {
  requires std::input_iterator<typename T::iterator>;
  requires std::input_iterator<typename T::const_iterator>;
  { a.size() }
  ->std::same_as<typename T::size_type>;
};

template <container Cont>
std::vector<std::pair<
    typename std::iterator_traits<typename Cont::iterator>::reference, int>>
enumerate(Cont &arr) {
  std::vector<std::pair<
      typename std::iterator_traits<typename Cont::iterator>::reference, int>>
      out(arr.size());
  int i = 0;
  for (auto &elem : arr) {
    out[i].first = elem;
    out[i].second = i;
    ++i;
  }
  return out;
}

template <std::input_iterator Iter_t>
void enumerate(
    Iter_t begin, Iter_t end,
    std::function<void(typename std::iterator_traits<Iter_t>::reference, int)>
        f) {
  int i = 0;
  for (; begin != end; ++begin, ++i)
    f(*begin, i);
}
template <std::input_iterator Iter_t>
void enumerate(
    Iter_t begin, Iter_t end,
    std::function<void(typename std::iterator_traits<Iter_t>::value_type, int)>
        f) {
  int i = 0;
  for (; begin != end; ++begin, ++i)
    f(*begin, i);
}

template <std::forward_iterator Iter1_t, std::forward_iterator Iter2_t>
requires(Iter1_t{} == Iter2_t{}) bool isPartOf(Iter1_t first1, Iter1_t end1,
                                               Iter2_t first2, Iter2_t end2) {
  return std::find_if_not(
             first2, end2,
             [first1, end1](
                 const typename std::iterator_traits<Iter2_t>::reference elem) {
               return std::find(first1, end1, elem) != end1;
             }) == end2;
}
} // namespace experimental