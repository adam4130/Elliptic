#ifndef SECP256K1_H 
#define SECP256K1_H 

#include "curve.h"

namespace elliptic {

    class Secp256k1 : public Curve {
    private:
        static const int A, B;
        static const std::string PRIME, ORDER;

        mpz_class convertHex(const std::string& hexString) const;

    public:
        Secp256k1() : Curve(A, B, convertHex(PRIME)) {}

        mpz_class getOrder() const { return convertHex(ORDER); };
    };
    
}

#endif
