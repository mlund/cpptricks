#undef DOCTEST_CONFIG_DISABLE

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#define DOCTEST_CONFIG_SUPER_FAST_ASSERTS

#include <doctest/doctest.h>
#include <algorithm>
#include <iostream>
#include <assert.h>
#include <vector>
#include <list>
#include <array>
#include <set>
#include <numeric>
#include <cmath>
#include <range/v3/all.hpp>

#include "pairwise_iterator.h"
#include "invsqrt.h"
#include "stl_eigen_facade.h"

