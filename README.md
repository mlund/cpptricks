# cpptricks

Collection of C++ snippets and header files that could be useful.
Build and test with:

~~~ bash
cmake .
make test
./test
~~~

## Description

- `invsqrt.h`. Quake fast inverse square root for floats/doubles.
- `stl_eigen_facade.h`. Access vector of structures as Eigen objects
- `pairwise_iterator.h`.
   Generates an iterable object pointing to all _unique_
   pairs in one container or between two containers. Dereferencing
   returns a `std::tuple` holding (const) references to the underlying data.
   ~~~ cpp
   std::vector<std::string> v = {"one","two", "three"};
   for (auto [i,j] : internal_pairs(v)):
   std::cout << i << "-" << j << " "; // --> "one-two" "one-three" "two-three"
   ~~~
   (similar to Python's
   [`itertools.combinations`](https://docs.python.org/2/library/itertools.html#itertools.combinations))
   ~~~ cpp
   std::vector<std::string> v1 = {"one","two"}, v2 = {"three"};
   for (auto [i,j] : cartesian_product(v1,v2)):
      std::cout << i << "-" << j << " "; // --> "one-three" "two-three"
   ~~~
   (similar to range-v3's
   [`cartesian_product`](https://www.fluentcpp.com/2017/04/14/understand-ranges-better-with-the-new-cartesian-product-adaptor/))
