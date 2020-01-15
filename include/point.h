#ifndef POINT_H
#define POINT_H

#include <cstddef> // std::size_t
#include <ostream> // std::ostream

#include <gmpxx.h>

namespace elliptic {

    class Point {
    public:
        Point() : x_(0), y_(0) {}
        Point(mpz_class x, mpz_class y) : x_(x), y_(y) {}

        mpz_class getX() const { return x_; }
        mpz_class getY() const { return y_; }

        bool isZero() const { return sgn(x_) == 0 && sgn(y_) == 0; }
        bool operator==(const Point &p) const { return cmp(x_, p.x_) == 0 && cmp(y_, p.y_) == 0; }
    private:
        mpz_class x_, y_;
    };

    std::ostream& operator<<(std::ostream& out, const Point& p);

    struct PointHasher {
        std::size_t operator()(const Point& p) const;
    };
}

#endif

