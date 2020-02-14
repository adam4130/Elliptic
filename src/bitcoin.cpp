#include "bitcoin.h"

#include <algorithm> // std::transform
#include <cstdlib>   // std::system
#include <stdexcept> // std::runtime_error, std::invalid_argument

const int elliptic::Bitcoin::HEX_LENGTH = 64;
const int elliptic::Bitcoin::WIF_LENGTH = 51;
const int elliptic::Bitcoin::COMPRESSED = 66;
const int elliptic::Bitcoin::UNCOMPRESSED = 130;

const std::string elliptic::Bitcoin::BASE_POINT = "0479BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798483ADA7726A3C4655DA4FBFC0E1108A8FD17B448A68554199C47D08FFB10D4B8";

/**
 * Generates a paper wallet PDF using LaTeX from a given private key.
 */
void elliptic::Bitcoin::paperWallet(const std::string& privateKey, bool compressed) const {
    std::string privateHex = convertToPrivateHex(privateKey);
    std::string publicKey = privateHexToPublicKey(privateHex, compressed);
    std::string address = publicKeyToAddress(publicKey);
    std::string WIF = privateHexToWIF(privateHex, compressed);

    std::string command = "cd LaTeX/; sh generate.sh " + address + " " + WIF;
    if (std::system(command.c_str()) != 0) {
        throw std::runtime_error("Unable to generate paper wallet");
    }
}

/**
 * Generates a hexadecimal private key using the OpenSSL library.
 */
std::string elliptic::Bitcoin::generatePrivateHex() const {
    std::string privateKey = hash_.getRandom(HEX_LENGTH / 2);
    if (!validPrivateHex(privateKey)) {
        throw std::runtime_error("Generated private key is invalid");
    }

    return toUpperCase(privateKey);
}

/**
 * Converts a hexadecimal public key (compressed or uncompressed) into a Point.
 */
elliptic::Point elliptic::Bitcoin::getPoint(const std::string& point) const {
    std::size_t length = point.length();
    if (length != COMPRESSED && length != UNCOMPRESSED) {
        throw std::invalid_argument("Length of point must be either " +
            std::to_string(COMPRESSED) + " or " + std::to_string(UNCOMPRESSED));
    }

    std::string key = point;
    if (length == COMPRESSED) {
        key = uncompressPublicKey(key);
    }

    if (key.substr(0, 2).compare("04") != 0) {
        throw std::invalid_argument("Compression byte is incorrect");
    }

    mpz_class x, y;
    if (x.set_str(key.substr(2, HEX_LENGTH), 16) != 0
            || y.set_str(key.substr(COMPRESSED, HEX_LENGTH), 16) != 0) {
        throw std::invalid_argument("Point is not a valid hexadecimal string");
    }

    Point p = Point(x, y);
    if (!curve_->hasPoint(p)) {
        throw std::invalid_argument("Point is not on the curve");
    }

    return p;
}

/**
 * Converts WIF (uncompressed or compressed) and dice (99 digits [0-5]) keys to
 * hexadecimal strings and verifies they valid.
 */
std::string elliptic::Bitcoin::convertToPrivateHex(const std::string& privateKey) const {
    std::size_t length = privateKey.length();
    std::string privateHex = privateKey;
    if (length == WIF_LENGTH || length == WIF_LENGTH + 1) {
        privateHex = WIFToPrivateHex(privateKey);
    } else if (length > HEX_LENGTH) {
        privateHex = diceToPrivateHex(privateKey);
    }

    if (!validPrivateHex(privateHex)) {
        throw std::invalid_argument("Private key is invalid");
    }

    return privateHex;
}

/**
 * Converts hexadecimal private keys to WIF (wallet import format) private keys.
 */
std::string elliptic::Bitcoin::privateHexToWIF(const std::string& privateKey,
        bool compressed) const {
    if (!validPrivateHex(privateKey)) {
        throw std::invalid_argument("Private key is invalid");
    }

    std::string WIF = "80" + privateKey;
    if (compressed) {
        WIF += "01";
    }

    std::string sha = hash_.sha256(hash_.sha256(WIF));
    return base58_.hexToBase58(WIF + sha.substr(0, 8));
}

/**
 * Converts WIF (uncompressed or compressed) private keys to hexadecimal
 * strings.
 */
std::string elliptic::Bitcoin::privateHexToPublicKey(const std::string& privateKey,
        bool compressed) const {
    if (!validPrivateHex(privateKey)) {
        throw std::invalid_argument("Private key is invalid");
    }

    mpz_class k;
    k.set_str(privateKey, 16);

    Point p = curve_->multiply(getBasePoint(), k);

    std::string publicKey = "04" + pad(p.getX().get_str(16), HEX_LENGTH)
        + pad(p.getY().get_str(16), HEX_LENGTH);

    if (compressed) {
        publicKey = compressPublicKey(publicKey);
    }

    return toUpperCase(publicKey);
}

/**
 * Converts a hexadecimal public key to an address.
 */
std::string elliptic::Bitcoin::publicKeyToAddress(const std::string& publicKey) const {
    getPoint(publicKey); // Throws exception if public key is not valid

    std::string sha = hash_.sha256(publicKey);
    std::string rip = hash_.ripemd160(sha);
    rip = "00" + rip;
    sha = hash_.sha256(hash_.sha256(rip));
    rip += sha.substr(0, 8);

    return base58_.hexToBase58(rip);
}

/**
 * Converts a compressed hexadecimal public key to a uncompressed hexadecimal
 * public key.
 */
std::string elliptic::Bitcoin::uncompressPublicKey(const std::string& compressed) const {
    if (compressed.length() != COMPRESSED || compressed[0] != '0') {
        throw std::invalid_argument("Compressed public key is invalid");
    }

    char compression = compressed[1];
    if (compression != '2' && compression != '3') {
        throw std::invalid_argument("Compression byte is invalid");
    }

    mpz_class x, y, r, P;
    if (x.set_str(compressed.substr(2, HEX_LENGTH), 16) != 0) {
        throw std::invalid_argument("Compressed hexadecimal public key is invalid");
    }

    P = curve_->getPrime();
    mpz_powm_ui(r.get_mpz_t(), x.get_mpz_t(), 3, P.get_mpz_t());
    r += curve_->getA()*x + curve_->getB();
    mpz_mod(r.get_mpz_t(), r.get_mpz_t(), P.get_mpz_t());
    y = curve_->squareRoot(r);

    bool even = mpz_even_p(y.get_mpz_t()) != 0;
    if ((compression == '2' && !even) || (compression == '3' && even)) {
        mpz_neg(y.get_mpz_t(), y.get_mpz_t());
        mpz_mod(y.get_mpz_t(), y.get_mpz_t(), P.get_mpz_t());
    }

    if (!curve_->hasPoint(Point(x, y))) {
        throw std::invalid_argument("Public key is not on curve");
    }

    std::string coord = pad(x.get_str(16), HEX_LENGTH) + pad(y.get_str(16), HEX_LENGTH);
    return toUpperCase("04" + coord);
}

/**
 * Converts a uncompressed hexadecimal public key to a compressed hexadecimal
 * public key.
 */
std::string elliptic::Bitcoin::compressPublicKey(const std::string& uncompressed) const {
    Point p = getPoint(uncompressed);
    std::string x = toUpperCase(pad(p.getX().get_str(16), HEX_LENGTH));

    if (mpz_even_p(p.getY().get_mpz_t()) != 0) {
        return "02" + x;
    } else {
        return "03" + x;
    }
}

/**
 * Verifies that hexadecimal private key is the correct length and has value
 * between 0 and the order of the elliptic curve.
 */
bool elliptic::Bitcoin::validPrivateHex(const std::string& privateKey) const {
    if (privateKey.length() != HEX_LENGTH) {
        return false;
    }

    mpz_class key;
    if (key.set_str(privateKey, 16) != 0) {
        return false;
    }

    // 0 < key < N (order)
    if (sgn(key) <= 0 || cmp(key, curve_->getOrder()) >= 0) {
        return false;
    }

    return true;
}

/**
 * Verifies that a WIF private key has the correct compression byte and length.
 */
bool elliptic::Bitcoin::validWIF(const std::string& WIF) const {
    std::size_t length = WIF.length();
    if (length != WIF_LENGTH && length != WIF_LENGTH + 1) {
        return false;
    }

    char compression = WIF[0];
    if (length == WIF_LENGTH && compression != '5') {
        return false;
    } else if (length == WIF_LENGTH + 1 && compression != 'K'
            && compression != 'L') {
        return false;
    }

    return true;
}

/**
 * Converts a hexadecimal private key to hexadecimal public key with matching
 * compression. If a given private key has the compressed WIF format, the
 * compressed public key will be returned and vise-versa.
 */
std::string elliptic::Bitcoin::WIFToPrivateHex(const std::string& WIF) const {
    if (!validWIF(WIF)) {
        throw std::invalid_argument("WIF private key is invalid");
    }

    std::string hex = base58_.base58ToHex(WIF);
    std::string checkSum = hex.substr(hex.length() - 8, 8);
    hex.erase(hex.length() - 8);
    std::string sha = hash_.sha256(hash_.sha256(hex));
    if (checkSum.compare(sha.substr(0, 8)) != 0) {
        throw std::invalid_argument("SHA-256 checksum is incorrect");
    }

    hex.erase(0, 2);
    char compression = WIF[0];
    if (compression == 'K' || compression == 'L') {
        hex.erase(hex.length() - 2);
    }

    return toUpperCase(hex);
}

/**
 * Converts a dice private key (99 digits [0-5]) to a hexadecimal string.
 */
std::string elliptic::Bitcoin::diceToPrivateHex(const std::string& base6) const {
    if (base6.length() != 99) {
        throw std::invalid_argument("Dice key conversion requires 99 base-6 digits, "
            + std::to_string(base6.length()) + " digits provided");
    }

    mpz_class n = 0;
    for (char c : base6) {
        if (c < '0' || c > '5') {
            throw std::invalid_argument("Base-6 key is invalid");
        }

        n *= 6;
        n += c - '0';
    }

    std::string privateHex = pad(n.get_str(16), HEX_LENGTH);
    return toUpperCase(privateHex);
}

/**
 * Pads the beginning of string with zeros with the final length size being the
 * given length.
 */
std::string elliptic::Bitcoin::pad(const std::string& input, std::size_t length) const {
    if (length <= input.length()) {
        return input;
    }

    return std::string(length - input.length(), '0').append(input);
}

/**
 * Converts a string to upper case.
 */
std::string elliptic::Bitcoin::toUpperCase(std::string input) const {
    std::transform(input.begin(), input.end(), input.begin(), ::toupper);

    return input;
}

