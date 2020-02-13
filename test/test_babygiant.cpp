#include <boost/test/unit_test.hpp>

#include "babygiant.h"

struct F {
    elliptic::BabyGiant babyGiant;
    elliptic::Curve curve;
    elliptic::Point G;

    F() : curve(0, 7, 37), G(16, 12) {}
};

BOOST_FIXTURE_TEST_SUITE(babygiant, F)

BOOST_AUTO_TEST_CASE(logarithm) {
    elliptic::Point P(9, 25);

    // Solve P = kG
    mpz_class k = babyGiant.discreteLogarithm(curve, G, P);
    BOOST_CHECK_EQUAL(k, 17);

    elliptic::Point Q = curve.multiply(G, k);
    BOOST_CHECK_EQUAL(P.getX(), Q.getX());
    BOOST_CHECK_EQUAL(P.getY(), Q.getY());
}

BOOST_AUTO_TEST_CASE(logarithm_identity) {
    mpz_class k = babyGiant.discreteLogarithm(curve, G, G);
    BOOST_CHECK_EQUAL(k, 1);
}

BOOST_AUTO_TEST_SUITE_END()

