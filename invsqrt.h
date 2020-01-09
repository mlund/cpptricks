#pragma once

/**
 * @brief Fast inverse square-root approximation
 *
 * Modified to work with both double and float and with one (less precise) or
 * two (more precise) iterations. Template conditionals are optimized out at
 * compile time (ML, 2019)
 *
 * @note Code comments supposedly from the original Quake III Arena code
 */
template <typename T, char iterations = 2> inline T inv_sqrt(T x) {
    static_assert(std::is_floating_point<T>::value, "T must be floating point");
    static_assert(iterations == 1 or iterations == 2, "itarations must equal 1 or 2");
    typedef typename std::conditional<sizeof(T) == 8, std::int64_t, std::int32_t>::type Tint;
    T y = x;
    T x2 = y * 0.5;
    Tint i = *(Tint *)&y;                                              // evil floating point bit level hacking
    i = (sizeof(T) == 8 ? 0x5fe6eb50c7b537a9 : 0x5f3759df) - (i >> 1); // what the fuck?
    y = *(T *)&i;
    y = y * (1.5 - (x2 * y * y)); // 1st iteration
    if (iterations > 1)
        y = y * (1.5 - (x2 * y * y)); // 2nd iteration, this can be removed
    return y;
}

#ifdef DOCTEST_LIBRARY_INCLUDED
TEST_CASE_TEMPLATE("inv_sqrt", T, double, float) {
    std::vector<T> vals = {0.23, 3.3, 10.2, 100.45, 512.06};
    for (auto x : vals)
        CHECK(inv_sqrt<T>(x) == doctest::Approx(1.0 / std::sqrt(x)));
}
#endif

