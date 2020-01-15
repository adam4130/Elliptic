#ifndef HASH_H
#define HASH_H

#include <cstddef> // std::size_t
#include <cstdint> // std::uint_8
#include <string>  // std::string
#include <vector>  // std::vector

namespace elliptic {

    class Hash {
    public:
        std::string sha256(const std::string& input) const;
        std::string ripemd160(const std::string& input) const;
        std::string getRandom(std::size_t bytes) const;
    private:
        std::vector<std::uint8_t> hexToByte(const std::string& input) const;
        std::string byteToHex(std::uint8_t* input, std::size_t length) const;
    };

}

#endif

