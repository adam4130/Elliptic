#ifndef SECP256K1_H 
#define SECP256K1_H 

#include "curve.h"

namespace elliptic {

    class Secp256k1 : public Curve {
        private:
        static const int A, B;
        static const std::string PRIME, ORDER;

        public:
        Secp256k1() : Curve(A, B, getPrime()) {}

        mpz_class getPrime();
        mpz_class getOrder();
    };
    
}

#endif
