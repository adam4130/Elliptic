#ifndef CURVE_H
#define CURVE_H 

#include "point.h"

namespace elliptic {

    class Curve {
    private:
        mpz_class prime_;
        int a_, b_;

    public:
        Curve(int a, int b, mpz_class prime);

        int getA() const { return a_; }
        int getB() const { return b_; }
        mpz_class getPrime() const { return prime_; }
        mpz_class getOrder();

        bool hasPoint(const Point& p);

        Point add(Point p, Point q);
        Point multiply(Point p);

        mpz_class inverse(const mpz_class& op);
        mpz_class squareRoot(const mpz_class& op);
    };

}

#endif
