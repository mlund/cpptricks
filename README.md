# cpptricks

Collection of C++ snippets and header files that could be useful.
Build and test with:

~~~ bash
cmake .
make test
./test
~~~

## `pairwise_iterator.h`

This generates an iterable object pointing to all _unique_
pairs in one container or between two containers. Dereferencing the iterator
returns a `std::tuple` holding (const) references to the underlying
data. The iterators can be conveniently used with STL algorithms.

Pairs in a single container - note that self-terms are excluded:

~~~ cpp
std::vector<std::string> v = {"one","two", "three"};
for (auto [i,j] : internal_pairs(v)):
   std::cout << i << "-" << j << " "; // --> "one-two" "one-three" "two-three"
~~~

(similar to Python's
[`itertools.combinations`](https://docs.python.org/2/library/itertools.html#itertools.combinations))

Pairs between two containers:

~~~ cpp
std::vector<std::string> v1 = {"one","two"}, v2 = {"three"};
for (auto [i,j] : cartesian_product(v1,v2)):
   std::cout << i << "-" << j << " "; // --> "one-three" "two-three"
~~~

(similar to range-v3's
[`cartesian_product`](https://www.fluentcpp.com/2017/04/14/understand-ranges-better-with-the-new-cartesian-product-adaptor/))
