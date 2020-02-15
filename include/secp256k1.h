#ifndef SECP256K1_H
#define SECP256K1_H

#include "curve.h"

namespace Elliptic {

    class Secp256k1 : public Curve {
    public:
        Secp256k1() : Curve(A, B, convertHex(PRIME)) {}

        mpz_class getOrder() const { return convertHex(ORDER); };
    private:
        static const int A, B;
        static const std::string PRIME, ORDER;

        static mpz_class convertHex(const std::string& hexString);
    };

}

#endif

