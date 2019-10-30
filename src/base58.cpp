#include "base58.h"

#include <algorithm> // std::reverse
#include <sstream>   // std::stringstream
#include <stdexcept> // std::invalid_argument

#include <gmpxx.h>

const std::string elliptic::Base58::BASE58 = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";

/**
 * Converts a hexadecimal string to Base58.
 */
std::string elliptic::Base58::hexToBase58(const std::string& input) const {
    mpz_class n;
    if (n.set_str(input, 16) != 0) {
        throw std::invalid_argument(input + " is an invalid hex string"); 
    }

    std::stringstream ss;
    while (sgn(n) > 0) { // n > 0
        // n = n / 58
        ss << BASE58[mpz_tdiv_q_ui(n.get_mpz_t(), n.get_mpz_t(), 58)];
    }

    for (std::size_t i = 0, length = input.length() - 1; i < length; i += 2) {
        if (input[i] != '0' || input[i+1] != '0') {
            break;
        }
    
        ss << BASE58[0];
    }

    std::string address = ss.str();
    std::reverse(address.begin(), address.end());
    return address;
}

/**
 * Converts a Base58 string to hexadecimal string.
 */
std::string elliptic::Base58::base58ToHex(const std::string& input) const {
    mpz_class n;
    n = 0;
    for (char c : input) {
        std::size_t index = BASE58.find(c);
        if (index == std::string::npos) {
            throw std::invalid_argument(input + " is an invalid Base58 string");
        }

        n *= 58;
        n += index;
    }

    return n.get_str(16);
}
