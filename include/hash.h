#ifndef HASH_H
#define HASH_H

#include <cstdint> // uint_8
#include <string>
#include <vector>

namespace elliptic {

    class Hash {
    private:
        std::vector<uint8_t> hexToByte(const std::string& input);
        std::string byteToHex(uint8_t* input, int length);

    public:
        std::string sha256(const std::string& input);
        std::string ripemd160(const std::string& input);
    };

}

#endif
