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
        bool equals(const Point& p);

        mpz_class getX() const { return x_; }
        mpz_class getY() const { return y_; }
    };

    std::ostream& operator<<(std::ostream& out, const Point& p);

}

#endif
