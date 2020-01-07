# cpptricks

Collection of C++ snippets and header files that could be useful.
Build and test with:

~~~ bash
cmake .
make test
./test
~~~

## `pairwise_iterator.h`

This generates an iterateable object (begin, end) to all unique
pairs in one or between two containers. Deferenrencing the iterator
returns a `std::tuple` holding (const) references to the underlying
data.

Examples:

~~~ cpp
std::vector<std::string> v = {"one","two", "three"};
for (auto [i,j] : internal_pairs(v)):
   std::cout << i << "-" << j << " "; // --> "one-two" "one-three" "two-three"
~~~

or

~~~ cpp
std::vector<std::string> v1 = {"one","two"}, v2 = {"three"};
for (auto [i,j] : external_pairs(v1,v2)):
   std::cout << i << "-" << j << " "; // --> "one-three" "two-three"
~~~
