#include "babygiant.h"

#include <cstdlib>       // std::rand, std::srand
#include <ctime>         // std::time
#include <stdexcept>     // std::invalid_argument
#include <unordered_map> // std::unordered_map

const long elliptic::BabyGiant::MEMORY_LIMIT = 50000000;

/**
 * Get a uniform random number in 0 to n - 1, inclusive. Seeding is done by the
 * system time.
 */
mpz_class elliptic::BabyGiant::getRandom(mpz_class n) const {
    gmp_randstate_t state;
    gmp_randinit_mt(state);

    std::srand(std::time(NULL));
    gmp_randseed_ui(state, std::rand());

    mpz_class random;
    mpz_urandomm(random.get_mpz_t(), state, n.get_mpz_t());

    gmp_randclear(state);

    return random;
}

/**
 * Computes a discrete logarithm on the given elliptic curve i.e., finds k such
 * that kG = P. The basic algorithm has space and time complexity of O(\sqrt{n})
 * where n is the order of the curve. Completeness is not guaranteed for \sqrt{n}
 * greater than the memory limit.
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

    mpz_class r = getRandom(m);
    Point mG = curve_->multiply(G, m);
    Point imG = curve_->multiply(mG, r);
    for (mpz_class i = r; i <= m; i++) {
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
