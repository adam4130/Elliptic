#ifndef BASE58_H
#define BASE58_H

#include <string>

namespace elliptic {
  
    class Base58 {
    private:
        static const std::string BASE58;

    public:
        std::string hexToBase58(const std::string& input);
        std::string base58ToHex(const std::string& input); 
    };

}

#endif
