#ifndef BASE58_H
#define BASE58_H

#include <string>

namespace Elliptic {

    namespace Base58 {

        extern const std::string BASE58;

        std::string hexToBase58(const std::string& input);
        std::string base58ToHex(const std::string& input);

    }

}

#endif

