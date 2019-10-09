#include "base58.h"

#include <algorithm> // std::reverse
#include <sstream>   // std::stringstream
#include <stdexcept> // std::invalid_argument

#include <gmpxx.h>

const std::string base58 = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";

/**
 * Checks is a given string is make of Base58 characters i.e., only contains:
 * 123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz.
 */
bool elliptic::isBase58(const std::string& input) {
    for (char c : input) {
        if (base58.find(c) == std::string::npos) {
            return false;
        }
    }

    return true;
}

/**
 * Converts a hexadecimal string to Base58.
 */
std::string elliptic::hexToBase58(const std::string& input) {
    mpz_class n, r;
    if (n.set_str(input, 16) != 0) {
        throw std::invalid_argument(input + " is an invalid hex string"); 
    }

    std::stringstream ss;
    while (sgn(n) == 1) { // n > 0
        ss << base58[mpz_tdiv_qr_ui(n.get_mpz_t(), r.get_mpz_t(), n.get_mpz_t(), 58)];
    }

    for (size_t i = 0, length = input.length()-1; i < length; i += 2) {
        if (input[i] != '0' || input[i+1] != '0') {
            break;
        }
    
        ss << base58[0];
    }

    std::string address = ss.str();
    std::reverse(address.begin(), address.end());
    return address;
}

/**
 * Converts a Base58 string to hexadecimal string.
 */
std::string elliptic::base58ToHex(const std::string& input) { 
    mpz_class n;
    n = 0;
    for (char c : input) {
        size_t index = base58.find(c);
        if (index == std::string::npos) {
            throw std::invalid_argument(input + " is an invalid Base58 string");
        }

        n *= 58;
        n += index;
    }

    return n.get_str(16);
}
