#include <iostream>

#include "bitcoin.h"

using namespace Elliptic;

int main() {
    Bitcoin bitcoin;

    // Generate a new uncompressed paper wallet
    bitcoin.paperWallet(bitcoin.generatePrivateHex(), false);

    return 0;
}

