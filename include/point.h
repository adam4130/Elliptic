#ifndef POINT_H
#define POINT_H 

#include <ostream>

#include <gmpxx.h>

namespace elliptic {

    class Point {
    private:
        mpz_class x_, y_;

    public:
        Point();
        Point(mpz_class x, mpz_class y);

        bool isZero();

        mpz_class getX() const { return x_; }
        mpz_class getY() const { return y_; }

        bool operator==(const Point &p) const { return cmp(x_, p.x_) == 0 && cmp(y_, p.y_) == 0; }
    };

    std::ostream& operator<<(std::ostream& out, const Point& p);

}

#endif
