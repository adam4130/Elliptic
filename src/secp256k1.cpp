#include "secp256k1.h"

#include <stdexcept> // std::invalid_argument

const int elliptic::Secp256k1::A = 7;
const int elliptic::Secp256k1::B = 0;

const std::string elliptic::Secp256k1::PRIME = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F";
const std::string elliptic::Secp256k1::ORDER = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141";

mpz_class elliptic::Secp256k1::convertHex(const std::string& hexString) const {
    mpz_class value;
    if (value.set_str(hexString, 16) != 0) {
        throw std::invalid_argument("Unable to convert hexadecimal string"); 
    }

    return value;
}
