#include <boost/test/unit_test.hpp>

#include "babygiant.h"

using namespace Elliptic;

struct F {
    Curve* curve;
    Point G;

    F() : curve(new Curve(0, 7, 37)), G(16, 12) {}
    ~F() { delete curve; }
};

BOOST_FIXTURE_TEST_SUITE(babygiant, F)

BOOST_AUTO_TEST_CASE(logarithm) {
    Point P(9, 25);

    // Solve P = kG
    mpz_class k = BabyGiant::discreteLogarithm(curve, G, P);
    BOOST_CHECK_EQUAL(k, 17);

    Point Q = curve->multiply(G, k);
    BOOST_CHECK_EQUAL(P.getX(), Q.getX());
    BOOST_CHECK_EQUAL(P.getY(), Q.getY());
}

BOOST_AUTO_TEST_CASE(logarithm_identity) {
    mpz_class k = BabyGiant::discreteLogarithm(curve, G, G);
    BOOST_CHECK_EQUAL(k, 1);
}

BOOST_AUTO_TEST_SUITE_END()

