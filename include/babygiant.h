#ifndef BABYGIANT_H
#define BABYGIANT_H

#include "curve.h"

namespace elliptic {

    Point babyGiant(const Curve& c, const mpz_class& n);

}

#endif
