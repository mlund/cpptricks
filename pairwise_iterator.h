/*
 *  Copyright (C) 2020  Mikael Lund (github.com/mlund/cpptricks)
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#pragma once
#include <tuple>
#include <type_traits>
#include <typeinfo>

namespace PairwiseIterator {

/**
 * @brief Iterator view to unique, self-avoiding pairs in a container
 *
 * The operation corresponds to iterating through a triangular matrix sans the trace.
 * Deferencing the iterator yields a tuple with (const) references to the
 * two data elements.
 *
 * Example:
 *
 * ~~~ cpp
 * std::vector<std::string> v = {"one","two", "three"};
 * for (auto [i,j] : internal_pairs(v)):
 *    cout << i << "-" << j << " ";
 * // --> "one-two" "one-three" "two-three"
 * ~~~
 *
 * @todo: implement non-const version
 */
template <class T, bool Const = std::is_const<T>::value> class internal_pairs {
  private:
    T &vec;
    struct iterator {
        // required for e.g. std::distance
        using iter = typename std::conditional<Const, typename T::const_iterator, typename T::iterator>::type;
        using pointer = void;
        using reference = typename std::conditional<Const, typename T::const_reference, typename T::reference>::type;
        using value_type = std::tuple<reference, reference>;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::forward_iterator_tag; // we can only move forward

        iter _end, i, j;
        iterator(iter end, iter i, iter j) : _end(end), i(i), j(j){};
        inline value_type operator*() { return {*i, *j}; }
        inline bool operator!=(const iterator &other) const { return (i != other.i) or (j != other.j); }
        inline iterator &operator++() {
            if (++j == _end)
                j = std::next(++i);
            return *this;
        }
    };

  public:
    internal_pairs(T &vec) : vec(vec) {}

    template <bool _Const = Const> std::enable_if_t<_Const, iterator> begin() const {
        return iterator(vec.end(), vec.begin(), std::next(vec.begin()));
    } // first pair

    template <bool _Const = Const> std::enable_if_t<_Const, iterator> end() const {
        return iterator(vec.end(), std::prev(vec.end()), vec.end());
    } // one iteration after last pair

    template <bool _Const = Const> std::enable_if_t<not _Const, iterator> begin() {
        return iterator(vec.end(), vec.begin(), std::next(vec.begin()));
    } // first pair

    template <bool _Const = Const> std::enable_if_t<not _Const, iterator> end() {
        return iterator(vec.end(), std::prev(vec.end()), vec.end());
    } // one iteration after last pair

    size_t size() const { return vec.size() * (vec.size() - 1) / 2; }
};

#ifdef DOCTEST_LIBRARY_INCLUDED
TEST_CASE_TEMPLATE("internal_pairs", T, std::vector<int>, std::list<int>, std::array<int, 4>) {
    T vec = {0, 1, 2, 3};
    internal_pairs pairs(vec);

    CHECK(pairs.size() == 6);
    CHECK(std::distance(pairs.begin(), pairs.end()) == 6);

    auto it = pairs.begin();
    CHECK(*it == std::tuple(0, 1));
    it.operator++();
    CHECK(*it == std::tuple(0, 2));
    it.operator++();
    CHECK(*it == std::tuple(0, 3));
    it.operator++();
    CHECK(*it == std::tuple(1, 2));
    it.operator++();
    CHECK(*it == std::tuple(1, 3));
    it.operator++();
    CHECK(*it == std::tuple(2, 3));

    SUBCASE("std::advance and std::next") {
        it = pairs.begin();
        it = std::next(it);
        CHECK(*it == std::tuple(0, 2));
        std::advance(it, 2);
        CHECK(*it == std::tuple(1, 2));
    }

    // can we modify?
    auto [i, j] = *pairs.begin();
    CHECK(i == 0);
    CHECK(j == 1);
    i = -1; // modify original vector
    CHECK(vec.front() == -1);
}
#endif

#ifdef RANGES_V3_VIEW_ZIP_HPP
#ifdef DOCTEST_LIBRARY_INCLUDED
TEST_CASE("internal_pairs_index") {
    // we can also generate all the indexes for a e.g. a random access container
    // @todo wrap in function - solve dangling references...
    using namespace ranges;
    int n = 4; // size of target container
    auto i = views::for_each(views::ints(0, n > 0 ? n - 1 : 0), [&](auto i) { return yield_from(views::repeat_n(i, n - 1 - i)); });
    auto j = views::ints(0, n > 0 ? n - 1 : 0) | views::for_each([&](auto i) { return views::ints(i + 1, n); });
    auto pairs = views::zip(i, j);

    CHECK(ranges::distance(pairs.begin(), pairs.end()) == 6);

    auto it = pairs.begin();
    CHECK(*it == std::pair(0, 1));
    it.operator++();
    CHECK(*it == std::pair(0, 2));
    it.operator++();
    CHECK(*it == std::pair(0, 3));
    it.operator++();
    CHECK(*it == std::pair(1, 2));
    it.operator++();
    CHECK(*it == std::pair(1, 3));
    it.operator++();
    CHECK(*it == std::pair(2, 3));
}
#endif
#endif

/**
 * Constant view to pairs in two containers
 * Calling `std::distance` is of O(N) complexity while `size` has constant complexity
 */
template <class Iter1, class Iter2> class cartesian_product {
  private:
    struct iterator {
        // these five are useful for stl
        using iterator_category = std::forward_iterator_tag; // we can only move forward
        using value_type = std::tuple<const typename Iter1::value_type &, const typename Iter2::value_type &>;
        using difference_type = std::ptrdiff_t;
        using reference = value_type &;
        using pointer = value_type *;

        Iter1 pos1, last1;
        Iter2 pos2, first2, last2;
        iterator(Iter1 first1, Iter1 last1, Iter2 first2, Iter2 last2)
            : pos1(first1), last1(last1), pos2(first2), first2(first2), last2(last2) {}

        inline value_type operator*() { return {*pos1, *pos2}; }
        inline bool operator!=(const iterator &other) const { return (pos1 != other.pos1) or (pos2 != other.pos2); }
        inline iterator &operator++() {
            if (++pos2 == last2) {
                pos2 = first2;
                pos1++;
                if (pos1 == last1)
                    pos2 = last2;
            }
            return *this;
        }
    };
    iterator _begin, _end;

  public:
    cartesian_product(Iter1 first1, Iter1 last1, Iter2 first2, Iter2 last2)
        : _begin(first1, last1, first2, last2), _end(last1, last1, last2, last2) {
        if (size() == 0)
            _begin = _end;
    }
    auto begin() const { return _begin; }
    auto end() const { return _end; }
    size_t size() const {
        return std::distance(_begin.pos1, _begin.last1) * std::distance(_begin.first2, _begin.last2);
    }
};

#ifdef DOCTEST_LIBRARY_INCLUDED
TEST_CASE_TEMPLATE("cartesian_product", T, std::vector<int>) {
    T vec1 = {0, 1, 3};
    T vec2 = {10, 20};
    cartesian_product pairs(vec1.begin(), vec1.end(), vec2.begin(), vec2.end());

    CHECK(pairs.size() == 6);
    CHECK(std::distance(pairs.begin(), pairs.end()) == 6);

    // sum second value from all pairs
    CHECK(std::accumulate(pairs.begin(), pairs.end(), 0, [](int sum, auto pair) { return sum + std::get<1>(pair); }) ==
          90);

    auto it = pairs.begin();
    CHECK(*it == std::tuple(0, 10));
    it.operator++();
    CHECK(*it == std::tuple(0, 20));
    it.operator++();
    CHECK(*it == std::tuple(1, 10));
    it.operator++();
    CHECK(*it == std::tuple(1, 20));
    it.operator++();
    CHECK(*it == std::tuple(3, 10));
    it.operator++();
    CHECK(*it == std::tuple(3, 20));

    vec1.clear();
    for (auto [i, j] : cartesian_product(vec1.begin(), vec1.end(), vec2.begin(), vec2.end())) {
        CHECK(false);
    }
}
#endif
} // namespace PairwiseIterator
