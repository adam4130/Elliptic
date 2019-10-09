#include "hash.h"

#include <iomanip> // std::setw(int)
#include <sstream> // std::stringstream()

#include <openssl/sha.h>
#include <openssl/ripemd.h>

/**
 * Converts a hexadecimal string to a uint8_t (byte) array pointer. The size
 * of the array is set to the given length reference. 
 */
std::vector<uint8_t> elliptic::Hash::hexToByte(const std::string& input) {
    if (input.length() % 2 != 0) {
        throw std::invalid_argument("Input must have an even number of characters");
    }

    int length = input.length() / 2;
    std::vector<uint8_t> output(length); 
    for (int i = 0; i < length; i++) {
        output.push_back(std::stoi(input.substr(i * 2, 2), 0, 16));
    }

    return output;
}

/**
 * Converts a uint8_t (byte) array pointer to a hexadecimal string.
 */
std::string elliptic::Hash::byteToHex(uint8_t* input, int length) {
    std::stringstream ss;
    for (int i = 0; i < length; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int) input[i];
    }

    return ss.str();
}

/**
 * Generates the SHA256 hash of a string using the OpenSSL library.
 */
std::string elliptic::Hash::sha256(const std::string& input) {
    std::vector<uint8_t> data = hexToByte(input);
    uint8_t output[SHA256_DIGEST_LENGTH];

    SHA256_CTX ctx;
    SHA256_Init(&ctx); 
    SHA256_Update(&ctx, &data[0], data.size());
    SHA256_Final(output, &ctx);

    return byteToHex(output, SHA256_DIGEST_LENGTH); 
}

/**
 * Generates the RIPEMD160 hash of a string using the OpenSSL library.
 */
std::string elliptic::Hash::ripemd160(const std::string& input) { 
    std::vector<uint8_t> data = hexToByte(input);
    uint8_t output[RIPEMD160_DIGEST_LENGTH];

    RIPEMD160_CTX ctx;
    RIPEMD160_Init(&ctx);
    RIPEMD160_Update(&ctx, &data[0], data.size());  
    RIPEMD160_Final(output, &ctx); 

    return byteToHex(output, RIPEMD160_DIGEST_LENGTH); 
}