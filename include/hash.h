#ifndef HASH_H
#define HASH_H

#include <cstddef> // std::size_t
#include <cstdint> // std::uint_8
#include <string>  // std::string
#include <vector>  // std::vector

namespace elliptic {

    class Hash {
    public:
        static std::string sha256(const std::string& input);
        static std::string ripemd160(const std::string& input);
        static std::string getRandom(std::size_t bytes);
    private:
        static std::vector<std::uint8_t> hexToByte(const std::string& input);
        static std::string byteToHex(std::uint8_t* input, std::size_t length);
    };

}

#endif

