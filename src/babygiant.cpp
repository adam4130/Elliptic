#include "babygiant.h"

#include <stdexcept>     // std::invalid_argument
#include <unordered_map> // std::unordered_map

const long elliptic::BabyGiant::MEMORY_LIMIT = 100000;

/**
 * Computes the discrete logarithm on the given elliptic curve i.e., finds k such
 * that kG = P. Has space and time complexity of O(\sqrt(n)) where n is the order
 * of the curve.
 */
mpz_class elliptic::BabyGiant::discreteLogarithm(const Point& P, const Point& G) const {
    if (!curve_->hasPoint(P) || !curve_->hasPoint(G)) {
        throw std::invalid_argument("Public key or base point is not on the curve");
    }

    mpz_class n = curve_->getOrder();
    mpz_class m = sqrt(n) + 1; // m = ceil(sqrt(n))

    long size = MEMORY_LIMIT;
    if (cmp(MEMORY_LIMIT, m) > 0) {
        size = m.get_si();
    }

    Point jG;
    std::unordered_map<Point, long, PointHasher> map;
    for (long j = 1; j <= size; j++) {
        jG = curve_->add(jG, G);
        map[jG] = j;
    }

    Point mG = curve_->multiply(G, m);
    Point imG = mG;
    for (long i = 1; i <= size; i++) {
        // Q = P - imG
        Point Q = curve_->add(P, curve_->negatePoint(imG));

        if (map.find(Q) != map.end()) {
            long j = map[Q];
            mpz_class k = i*m + j;
            mpz_mod(k.get_mpz_t(), k.get_mpz_t(), n.get_mpz_t());
            return k;
        }

        imG = curve_->add(imG, mG);
    }

    throw std::invalid_argument("Could not find k such that kG = P, increase memory limit");
}
