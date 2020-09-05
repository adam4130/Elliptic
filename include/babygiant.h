#ifndef BABYGIANT_H
#define BABYGIANT_H

#include <memory>        // std::unique_ptr
#include <unordered_map> // std::unordered_map
#include <utility>       // std::move

#include "curve.h"

namespace Elliptic {

    class BabyGiant {
    public:
        BabyGiant(std::unique_ptr<Curve> curve) : curve_(std::move(curve)) {}

        mpz_class discreteLogarithm(const Point& G, const Point& P);
    private:
        static const long MEMORY_LIMIT;
        static mpz_class getRandom(mpz_class n);

        std::unique_ptr<Curve> curve_;

        void populateTable(std::unordered_map<Point, long, PointHasher>& table,
                const Point& G, mpz_class m);
    };

}

#endif

