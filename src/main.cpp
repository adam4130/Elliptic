#include <iostream>

#include "bitcoin.h"

using namespace elliptic;

int main() {
    Bitcoin bitcoin;

    // Generate a new uncompressed paper wallet
    bitcoin.paperWallet(bitcoin.generatePrivateHex(), false);

    return 0;
}
