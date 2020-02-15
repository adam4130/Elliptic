#ifndef BABYGIANT_H
#define BABYGIANT_H

#include <unordered_map> // std::unordered_map

#include "curve.h"

namespace elliptic {

    class BabyGiant {
    public:
        static mpz_class discreteLogarithm(const Curve& curve, const Point& G, const Point& P);
    private:
        static const long MEMORY_LIMIT;

        static mpz_class getRandom(mpz_class n);
        static void populateTable(std::unordered_map<Point, long, PointHasher>& table,
            const Curve& curve, const Point& G, mpz_class m);
    };

}

#endif

