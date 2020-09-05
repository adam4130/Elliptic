#include <boost/test/unit_test.hpp>

#include <memory> // std::unique_ptr

#include "babygiant.h"

using namespace Elliptic;

struct F {
    BabyGiant babygiant;
    Point G;

    F() : babygiant(std::unique_ptr<Curve>(new Curve(0, 7, 37))), G(16, 12) {}
};

BOOST_FIXTURE_TEST_SUITE(babygiant, F)

BOOST_AUTO_TEST_CASE(logarithm) {
    Point P(9, 25);

    // Solve P = kG
    mpz_class k = babygiant.discreteLogarithm(G, P);
    BOOST_CHECK_EQUAL(k, 17);
}

BOOST_AUTO_TEST_CASE(logarithm_identity) {
    mpz_class k = babygiant.discreteLogarithm(G, G);
    BOOST_CHECK_EQUAL(k, 1);
}

BOOST_AUTO_TEST_SUITE_END()

