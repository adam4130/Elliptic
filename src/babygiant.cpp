#include "babygiant.h"

#include <cstdlib>       // std::rand, std::srand
#include <ctime>         // std::time
#include <stdexcept>     // std::invalid_argument

const long Elliptic::BabyGiant::MEMORY_LIMIT = 50000000;

/**
 * Computes a discrete logarithm on the given elliptic curve i.e., finds k such
 * that kG = P. The basic algorithm has space and time complexity of O(\sqrt{n})
 * where n is the order of the curve. Completeness is not guaranteed for \sqrt{n}
 * greater than the memory limit.
 */
mpz_class Elliptic::BabyGiant::discreteLogarithm(Curve* curve, const Point& G,
        const Point& P) {
    if (!curve->hasPoint(G) || !curve->hasPoint(P)) {
        throw std::invalid_argument("Base point or public key is not on the curve");
    }

    mpz_class n = curve->getOrder();
    mpz_class m = sqrt(n) + 1; // m = ceil(sqrt(n))

    std::unordered_map<Point, long, PointHasher> table;
    populateTable(table, curve, G, m);

    mpz_class r = getRandom(m);
    Point mG = curve->multiply(G, m);
    Point imG = curve->multiply(mG, r);
    for (mpz_class i = r; i < m + r; i++) {
        // Q = P - imG
        Point Q = curve->add(P, curve->negatePoint(imG));

        if (table.find(Q) != table.end()) {
            long j = table[Q];
            mpz_class k = i*m + j;
            mpz_mod(k.get_mpz_t(), k.get_mpz_t(), n.get_mpz_t());
            return k;
        }

        imG = curve->add(imG, mG);
    }

    throw std::invalid_argument("Could not find k such that kG = P, increase memory limit");
}

/**
 * Fills the given hash map with points, { jG | 1 <= j <= min(m, MEMORY_LIMIT) }, mapping
 * to their multiplier, j.
 */
void Elliptic::BabyGiant::populateTable(std::unordered_map<Point, long, PointHasher>& table,
        Curve* curve, const Point& G, mpz_class m) {
    long size = MEMORY_LIMIT;
    if (cmp(MEMORY_LIMIT, m) > 0) {
        size = m.get_si();
    }

    Point jG;
    for (long j = 1; j <= size; j++) {
        jG = curve->add(jG, G);
        table[jG] = j;
    }
}

/**
 * Get a uniform random number in 1 to n, inclusive. Seeding is done by the
 * system time. This method should NOT be used to generate random numbers for
 * private keys (consider using `getRandom` in "hash.h" which relies on /dev/random).
 */
mpz_class Elliptic::BabyGiant::getRandom(mpz_class n) {
    gmp_randstate_t state;
    gmp_randinit_mt(state);

    std::srand(std::time(NULL));
    gmp_randseed_ui(state, std::rand());

    mpz_class random;
    mpz_urandomm(random.get_mpz_t(), state, n.get_mpz_t());

    gmp_randclear(state);

    return random + 1; // Add 1 since `mpz_urandomm` generates 0 to n - 1
}

