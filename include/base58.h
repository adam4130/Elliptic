#ifndef BASE58_H
#define BASE58_H

#include <string>

namespace elliptic {
  
    bool isBase58(const std::string& input);
    std::string hexToBase58(const std::string& input);
    std::string base58ToHex(const std::string& input); 

}

#endif
