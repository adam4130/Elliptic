#include "hash.h"

#include <cctype>    // std::isxdigit
#include <iomanip>   // std::setfill, std::setw
#include <iostream>  // std::hex
#include <sstream>   // std::stringstream
#include <stdexcept> // std::invalid_argument

#include <openssl/sha.h>
#include <openssl/rand.h>
#include <openssl/ripemd.h>

/**
 * Generates the SHA256 hash of a string using the OpenSSL library.
 */
std::string elliptic::Hash::sha256(const std::string& input) const {
    std::vector<std::uint8_t> data = hexToByte(input);
    std::uint8_t output[SHA256_DIGEST_LENGTH];

    SHA256_CTX ctx;
    SHA256_Init(&ctx);
    SHA256_Update(&ctx, &data[0], data.size());
    SHA256_Final(output, &ctx);

    return byteToHex(output, SHA256_DIGEST_LENGTH);
}

/**
 * Generates the RIPEMD160 hash of a string using the OpenSSL library.
 */
std::string elliptic::Hash::ripemd160(const std::string& input) const {
    std::vector<std::uint8_t> data = hexToByte(input);
    std::uint8_t output[RIPEMD160_DIGEST_LENGTH];

    RIPEMD160_CTX ctx;
    RIPEMD160_Init(&ctx);
    RIPEMD160_Update(&ctx, &data[0], data.size());
    RIPEMD160_Final(output, &ctx);

    return byteToHex(output, RIPEMD160_DIGEST_LENGTH);
}

/**
 * Generates a hexadecimal string of random bytes using the OpenSSL library.
 */
std::string elliptic::Hash::getRandom(std::size_t bytes) const {
    std::uint8_t buf[bytes];
    if (RAND_bytes(buf, bytes) != 1) {
        throw std::runtime_error("OpenSSL unable to generate random bytes");
    }

    return byteToHex(buf, bytes);
}

/**
 * Converts a hexadecimal string to a uint8_t (byte) vector.
 */
std::vector<std::uint8_t> elliptic::Hash::hexToByte(const std::string& input) const {
    if (input.length() % 2 != 0) {
        throw std::invalid_argument("Input must have an even number of characters");
    }

    std::size_t length = input.length() / 2;
    std::vector<std::uint8_t> output;
    output.reserve(length);
    for (std::size_t i = 0; i < length; i++) {
        std::string value = input.substr(i * 2, 2);
        if (!std::isxdigit(value[0]) || !std::isxdigit(value[1])) {
            throw std::invalid_argument("Input is not a valid hexadecimal string");
        }

        output.push_back(std::stoi(value, 0, 16));
    }

    return output;
}

/**
 * Converts a uint8_t (byte) array pointer to a hexadecimal string.
 */
std::string elliptic::Hash::byteToHex(std::uint8_t* input, std::size_t length) const {
    std::stringstream ss;
    for (std::size_t i = 0; i < length; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int) input[i];
    }

    return ss.str();
}

