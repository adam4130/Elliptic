#include <iostream>

#include "bitcoin.h"

using namespace elliptic;

int main() {

    std::string privateHex = "E9873D79C6D87DC0FB6A5778633389F4453213303DA61F20BD67FC233AA33262";
    Bitcoin bitcoin;
    std::string publicKey = bitcoin.privateHexToPublicKey(privateHex, false); // Uncompressed
    std::cout << "Address: " << bitcoin.publicKeyToAddress(publicKey) << std::endl;

    return 0;
}
