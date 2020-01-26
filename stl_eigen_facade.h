#pragma once

#include <Eigen/Core>
#include <iterator>
#include <type_traits>

/**
 * @brief Eigen::Map facade to data members in STL container
 *
 * No data is copied and modifications of the Eigen object
 * modifies the original container and vice versa.
 *
 * Example:
 *
 *    struct Particle {
 *       Eigen::Vector3d pos;
 *       double charge;
 *    };
 *    std::vector<Particle> v(10);
 *    auto m1 = asEigenMatrix(v.begin, v.end(), &Particle::pos);    --> 10x3 maxtix view
 *    auto m2 = asEigenVector(v.begin, v.end(), &Particle::charge); --> 10x1 vector view
 */
template <typename dbl = double, class iter, class memberptr> auto asEigenMatrix(iter begin, iter end, memberptr m) {
    typedef typename std::iterator_traits<iter>::value_type T;
    static_assert(sizeof(T) % sizeof(dbl) == 0, "value_type size must multiples of double");
    const size_t s = sizeof(T) / sizeof(dbl);
    const size_t cols = sizeof(((T *)0)->*m) / sizeof(dbl);
    typedef Eigen::Matrix<dbl, Eigen::Dynamic, cols> Tmatrix;
    return Eigen::Map<Tmatrix, 0, Eigen::Stride<1, s>>((dbl *)&(*begin.*m), end - begin, cols).array();
}

template <typename dbl = double, class iter, class memberptr> auto asEigenVector(iter begin, iter end, memberptr m) {
    typedef typename std::iterator_traits<iter>::value_type T;
    static_assert(std::is_same<dbl &, decltype(((T *)0)->*m)>::value, "member must be a scalar");
    return asEigenMatrix<dbl>(begin, end, m).col(0);
}

#ifdef DOCTEST_LIBRARY_INCLUDED
TEST_CASE("asEigenMatrix") {
    struct Particle {
        Eigen::Vector3d pos={0,0,0};
        double charge=0;
        EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    };
    std::vector<Particle> v(4);
    v[0].pos.x()=5;
    v[1].pos.y()=10;
    v[2].pos.z()=2;
    auto m = asEigenMatrix(v.begin(), v.end(), &Particle::pos);

    using doctest::Approx;
    CHECK( m.cols()==3 );
    CHECK( m.rows()==4 );
    CHECK( m.row(0).x() == 5 );
    CHECK( m.row(1).y() == 10 );
    CHECK( m.row(2).z() == 2 );
    CHECK( m.sum() == 17);
    m.row(0).z()+=0.5;
    CHECK( v[0].pos.z() == Approx(0.5) );

    v[2].charge = 2;
    v[3].charge = -12;
    auto m2 = asEigenVector(v.begin()+1, v.end(), &Particle::charge);
    CHECK( m2.cols()==1 );
    CHECK( m2.rows()==3 );
    CHECK( m2.col(0).sum() == Approx(-10) );
}
#endif

