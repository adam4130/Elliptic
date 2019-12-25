#ifndef BITCOIN_H
#define BITCOIN_H

#include <memory> // std::unique_ptr

#include "base58.h"
#include "secp256k1.h"
#include "hash.h"

namespace elliptic {

    class Bitcoin {
    private:
        static const int HEX_LENGTH, WIF_LENGTH, COMPRESSED, UNCOMPRESSED;
        static const std::string BASE_POINT;

        std::unique_ptr<Curve> curve_;
        Base58 base58_;
        Hash hash_;

        std::string pad(const std::string& input, std::size_t length) const;
        std::string toUpperCase(std::string input) const;
        std::string WIFToPrivateHex(const std::string& WIF) const;
        std::string diceToPrivateHex(const std::string& base6) const;

        bool validPrivateHex(const std::string& privateKey) const;
        bool validWIF(const std::string& WIF) const;

    public:
        Bitcoin() : curve_(new Secp256k1()) {}

        Point getPoint(const std::string& point) const;
        Point getBasePoint() const { return getPoint(BASE_POINT); }

        void paperWallet(const std::string& privateKey, bool compressed) const;

        std::string generatePrivateHex() const;
        std::string convertToPrivateHex(const std::string& privateKey) const;
        std::string privateHexToWIF(const std::string& privateKey, bool compressed) const;
        std::string privateHexToPublicKey(const std::string& privateKey, bool compressed) const;
        std::string publicKeyToAddress(const std::string& publicKey) const;
        std::string uncompressPublicKey(const std::string& compressed) const;
        std::string compressPublicKey(const std::string& uncompressed) const;
    };

}

#endif
