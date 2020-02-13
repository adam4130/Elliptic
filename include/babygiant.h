#ifndef BABYGIANT_H
#define BABYGIANT_H

#include <unordered_map> // std::unordered_map

#include "curve.h"

namespace elliptic {

    class BabyGiant {
    public:
        mpz_class discreteLogarithm(const Curve& curve, const Point& G, const Point& P) const;
    private:
        static const long MEMORY_LIMIT;

        mpz_class getRandom(mpz_class n) const;
        void populateTable(std::unordered_map<Point, long, PointHasher>& table,
            const Curve& curve, const Point& G, mpz_class m) const;
    };

}

#endif

