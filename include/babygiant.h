#ifndef BABYGIANT_H
#define BABYGIANT_H

#include <unordered_map> // std::unordered_map

#include "curve.h"

namespace elliptic {

    class BabyGiant {
    public:
        BabyGiant(Curve* curve) : curve_(curve) {}
        mpz_class discreteLogarithm(const Point& P, const Point& G) const;
    private:
        static const long MEMORY_LIMIT;

        Curve* curve_;

        mpz_class getRandom(mpz_class n) const;
        void populateTable(std::unordered_map<Point, long, PointHasher>& table,
            const Point& G, mpz_class m) const;
    };

}

#endif

