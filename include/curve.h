#ifndef CURVE_H
#define CURVE_H 

#include "point.h"

namespace elliptic {

    class Curve {
    private:
        int a_, b_;
        mpz_class prime_;

    public:
        Curve(int a, int b, mpz_class prime);
        virtual ~Curve() {}

        int getA() const { return a_; }
        int getB() const { return b_; }

        mpz_class getPrime() const { return prime_; }
        virtual mpz_class getOrder() const;

        bool hasPoint(const Point& p) const;
        Point negatePoint(const Point &p) const;

        Point add(Point p, Point q) const;
        Point multiply(Point p) const;
        Point multiply(Point p, mpz_class n) const;

        mpz_class inverse(const mpz_class& op) const;
        mpz_class squareRoot(const mpz_class& op) const;
    };

}

#endif
