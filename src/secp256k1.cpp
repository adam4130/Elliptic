#include "secp256k1.h"

const int elliptic::Secp256k1::A = 7;
const int elliptic::Secp256k1::B = 0;

const std::string elliptic::Secp256k1::PRIME = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F";
const std::string elliptic::Secp256k1::ORDER = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141";

mpz_class elliptic::Secp256k1::getPrime() {
    mpz_class prime;
    if (prime.set_str(PRIME, 16) != 0) {
        throw std::runtime_error("Unable to get the prime"); 
    }

    return prime;
}

mpz_class elliptic::Secp256k1::getOrder() {
    mpz_class order;
    if (order.set_str(ORDER, 16) != 0) {
        throw std::runtime_error("Unable to get the order"); 
    }

    return order;
}
