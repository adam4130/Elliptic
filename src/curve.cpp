#include "curve.h"

#include <algorithm> // std::reverse
#include <cmath>     // std::pow
#include <stdexcept> // std::invalid_argument

Elliptic::Curve::Curve(int a, int b, mpz_class prime) {
    this->a_ = a;
    this->b_ = b;
    this->prime_ = prime;

    // \delta = 4a^3 + 27b^2
    double discriminant = 4*std::pow(a_, 3) + 27*std::pow(b_, 2);
    if (std::abs(discriminant) < 1e-12) {
        throw std::invalid_argument("Discriminant must be non-zero");
    }
}

/**
 * Naive order computation with O(p^2) complexity. Currently, this function must
 * be overridden with the known value when using with reasonable curves e.g.,
 * secp256k1.
 * TODO Implement Schoof's algorithm which computes the order in O(log^8 p) time.
 */
mpz_class Elliptic::Curve::getOrder() const {
    mpz_class order = 0;
    for (mpz_class x = 0; x < prime_; x++) {
        for (mpz_class y = 0; y < prime_; y++) {
            if (hasPoint(Point(x, y))) {
                order++;
            }
        }
    }

    return order + 1; // Add identity element
}

/**
 * Checks if the given point is on the curve, y^2 = x^3 + ax + b (mod p).
 */
bool Elliptic::Curve::hasPoint(const Point& p) const {
    mpz_class left, right;
    mpz_powm_ui(left.get_mpz_t(), p.getY().get_mpz_t(), 2, prime_.get_mpz_t());

    mpz_powm_ui(right.get_mpz_t(), p.getX().get_mpz_t(), 3, prime_.get_mpz_t());
    right += a_*p.getX() + b_;
    mpz_mod(right.get_mpz_t(), right.get_mpz_t(), prime_.get_mpz_t());

    return cmp(left, right) == 0;
}

/**
 * Negates a point the curve i.e., p = (x, y) => -p = (x, -y).
 */
Elliptic::Point Elliptic::Curve::negatePoint(const Point &p) const {
    mpz_class y = -p.getY();
    mpz_mod(y.get_mpz_t(), y.get_mpz_t(), prime_.get_mpz_t());

    return Point(p.getX(), y);
}

/**
 * Adds two Points on the curve, y^2 = x^3 + ax + b (mod p).
 */
Elliptic::Point Elliptic::Curve::add(Point p, Point q) const {
    // p + 0 = p
    if (q.isZero()) {
        return p;
    }

    // q + 0 = q
    if (p.isZero()) {
        return q;
    }

    if (cmp(p.getX(), q.getX()) == 0) {
        // p = q
        if (cmp(p.getY(), q.getY()) == 0) {
            return multiply(p);
        }

        // p = -q
        return Point();
    }

    mpz_class num = p.getY() - q.getY();
    mpz_class denom = p.getX() - q.getX();
    mpz_class lambda = num*inverse(denom);

    mpz_class x;
    mpz_powm_ui(x.get_mpz_t(), lambda.get_mpz_t(), 2, prime_.get_mpz_t());
    x -= p.getX() + q.getX();
    mpz_mod(x.get_mpz_t(), x.get_mpz_t(), prime_.get_mpz_t());

    mpz_class y = p.getX() - x;
    y *= lambda;
    y -= p.getY();
    mpz_mod(y.get_mpz_t(), y.get_mpz_t(), prime_.get_mpz_t());

    return Point(x, y);
}

/**
 * Doubles a Point on the curve, y^2 = x^3 + ax + b (mod p).
 */
Elliptic::Point Elliptic::Curve::multiply(Point p) const {
    if (p.isZero()) {
        return p;
    }

    mpz_class squared;
    mpz_powm_ui(squared.get_mpz_t(), p.getX().get_mpz_t(), 2, prime_.get_mpz_t());

    // lambda = (3*x^2 + a) / (2*y)
    mpz_class lambda = (3*squared + a_)*inverse(2*p.getY());

    mpz_class x;
    mpz_powm_ui(x.get_mpz_t(), lambda.get_mpz_t(), 2, prime_.get_mpz_t());
    x -= 2*p.getX();
    mpz_mod(x.get_mpz_t(), x.get_mpz_t(), prime_.get_mpz_t());

    mpz_class y = p.getX() - x;
    y *= lambda;
    y -= p.getY();
    mpz_mod(y.get_mpz_t(), y.get_mpz_t(), prime_.get_mpz_t());

    return Point(x, y);
}

/**
 * Computes q = np where n is a natural number greater than zero and p is point
 * on the curve, y^2 = x^3 + ax + b (mod p).
 */
Elliptic::Point Elliptic::Curve::multiply(Point p, mpz_class n) const {
    if (sgn(n) <= 0) {
        throw std::invalid_argument("n must be greater than 0");
    }

    std::string binary = n.get_str(2);
    std::reverse(binary.begin(), binary.end());

    Point q;
    for (char bit : binary) {
        if (bit == '1') {
            q = add(q, p);
        }

        p = multiply(p);
    }

    return q;
}

/**
 * Finds the inverse mod p using Fermat's little theorem.
 */
mpz_class Elliptic::Curve::inverse(const mpz_class& op) const {
    if (mpz_divisible_p(op.get_mpz_t(), prime_.get_mpz_t()) != 0) {
        throw std::invalid_argument("Inverse does not exist");
    }

    // Fermat's Little Theorem
    mpz_class inv, exp = prime_ - 2;
    mpz_powm_sec(inv.get_mpz_t(), op.get_mpz_t(), exp.get_mpz_t(), prime_.get_mpz_t());
    return inv;
}

/**
 * Finds the square root mod p. Only valid for p \equiv 3 (mod 4).
 */
mpz_class Elliptic::Curve::squareRoot(const mpz_class& op) const {
    if (mpz_fdiv_ui(prime_.get_mpz_t(), 4) != 3) {
        throw std::invalid_argument("P != 3 (mod 4)");
    }

    mpz_class sqr, exp = prime_ + 1;
    mpz_divexact_ui(exp.get_mpz_t(), exp.get_mpz_t(), 4);
    mpz_powm(sqr.get_mpz_t(), op.get_mpz_t(), exp.get_mpz_t(), prime_.get_mpz_t());
    return sqr;
}

