#ifndef HASH_H
#define HASH_H

#include <cstddef> // size_t
#include <cstdint> // uint_8
#include <string>  // std::string
#include <vector>  // std::vector

namespace elliptic {

    class Hash {
    private:
        std::vector<uint8_t> hexToByte(const std::string& input) const;
        std::string byteToHex(uint8_t* input, int length) const;

    public:
        std::string sha256(const std::string& input) const;
        std::string ripemd160(const std::string& input) const;

        std::string getRandom(size_t bytes) const;
    };

}

#endif
