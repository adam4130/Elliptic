#include <iostream>

#include "bitcoin.h"

using namespace elliptic;

int main() {
    Bitcoin bitcoin;
    bitcoin.paperWallet(bitcoin.generatePrivateHex(), false);
    return 0;
}
