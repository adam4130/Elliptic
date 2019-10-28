#include "point.h"

elliptic::Point::Point() {
    x_ = 0;
    y_ = 0;
}
  
elliptic::Point::Point(mpz_class x, mpz_class y) {
    x_ = x;
    y_ = y;
}

/**
 * Checks if this point equals (0, 0).
 */
bool elliptic::Point::isZero() {
    if (sgn(x_) == 0 && sgn(y_) == 0) {
        return true;
    }

    return false;
}

std::ostream& elliptic::operator<<(std::ostream& out, const Point& p) {
    out << '(' << p.getX() << ',' << p.getY() << ')';
    return out;
}
