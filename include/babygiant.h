#ifndef BABYGIANT_H
#define BABYGIANT_H

#include "curve.h"

namespace elliptic {

    class BabyGiant {
    private:
        static const long MEMORY_LIMIT;

        Curve* curve_;

    public:
        BabyGiant(Curve* curve) : curve_(curve) {}

        mpz_class discreteLogarithm(const Point& P, const Point& G) const;
    };

}

#endif
