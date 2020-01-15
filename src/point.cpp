#include "point.h"

#include <cstddef>    // size_t
#include <functional> // std::hash
#include <string>     // Std::string

#include <boost/functional/hash.hpp> // boost::hash_combine, boost::hash_value

std::ostream& elliptic::operator<<(std::ostream& out, const Point& p) {
    out << '(' << p.getX() << ',' << p.getY() << ')';
    return out;
}

std::size_t elliptic::PointHasher::operator()(const Point& p) const {
    const std::string x = p.getX().get_str(10);
    const std::string y = p.getY().get_str(10);

    std::size_t first = std::hash<std::string>()(x);
    std::size_t second = std::hash<std::string>()(y);

    std::size_t seed = 0;
    boost::hash_combine(seed, boost::hash_value(first));
    boost::hash_combine(seed, boost::hash_value(second));

    return seed;
}

