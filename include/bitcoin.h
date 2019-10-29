#ifndef BITCOIN_H
#define BITCOIN_H

#include "base58.h"
#include "secp256k1.h"
#include "hash.h"

namespace elliptic {

    class Bitcoin {
    private:
        static const int HEX_LENGTH, WIF_LENGTH, COMPRESSED, UNCOMPRESSED;
        static const std::string BASE_POINT;

        Curve* curve_;
        Base58 base58_;
        Hash hash_;
        
        std::string pad(const std::string& input, std::size_t length);
        std::string toUpperCase(std::string input);
        std::string WIFToPrivateHex(const std::string& WIF);
        std::string diceToPrivateHex(const std::string& base6);

        bool validPrivateHex(const std::string& privateKey);
        bool validWIF(const std::string& WIF);
        bool getPoint(const std::string& publicKey, Point& p);

    public:
        Bitcoin() : curve_(new Secp256k1()) {}
        ~Bitcoin() { delete curve_; }
        
        void paperWallet(const std::string& privateKey, bool compressed);
        
        std::string generatePrivateHex();
        std::string convertToPrivateHex(const std::string& privateKey);
        std::string privateHexToWIF(const std::string& privateKey, bool compressed);
        std::string privateHexToPublicKey(const std::string& privateKey, bool compressed);
        std::string publicKeyToAddress(const std::string& publicKey);
        std::string uncompressPublicKey(const std::string& compressed);
        std::string compressPublicKey(const std::string& uncompressed);
    };

}

#endif
