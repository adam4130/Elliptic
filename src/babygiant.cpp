#include "babygiant.h"

#include <cstdlib>       // std::rand, std::srand
#include <ctime>         // std::time
#include <stdexcept>     // std::invalid_argument

const long elliptic::BabyGiant::MEMORY_LIMIT = 50000000;

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

    std::unordered_map<Point, long, PointHasher> table;
    populateTable(table, G, m);

    mpz_class r = getRandom(m);
    Point mG = curve_->multiply(G, m);
    Point imG = curve_->multiply(mG, r);
    for (mpz_class i = r; i <= m; i++) {
        // Q = P - imG
        Point Q = curve_->add(P, curve_->negatePoint(imG));

        if (table.find(Q) != table.end()) {
            long j = table[Q];
            mpz_class k = i*m + j;
            mpz_mod(k.get_mpz_t(), k.get_mpz_t(), n.get_mpz_t());
            return k;
        }

        imG = curve_->add(imG, mG);
    }

    throw std::invalid_argument("Could not find k such that kG = P, increase memory limit");
}

/**
 * Fills the given hash map with points, { jG | 1 <= j <= min(m, MEMORY_LIMIT) }, mapping
 * to their multiplier, j.
 */
void elliptic::BabyGiant::populateTable(std::unordered_map<Point, long, PointHasher>& table,
        const Point& G, mpz_class m) const {
    long size = MEMORY_LIMIT;
    if (cmp(MEMORY_LIMIT, m) > 0) {
        size = m.get_si();
    }

    Point jG;
    for (long j = 1; j <= size; j++) {
        jG = curve_->add(jG, G);
        table[jG] = j;
    }
}

/**
 * Get a uniform random number in 0 to n - 1, inclusive. Seeding is done by the
 * system time. This method should NOT be used to generate random numbers for
 * private keys (consider using `getRandom` in "hash.h" which relies on /dev/random).
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

