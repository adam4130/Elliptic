#include <iostream>

#include "bitcoin.h"

using namespace std;
using namespace elliptic;

int main() {

    string privateHex = "E9873D79C6D87DC0FB6A5778633389F4453213303DA61F20BD67FC233AA33262";
    Bitcoin bitcoin;
    cout << bitcoin.privateHexToPublicKey(privateHex, false) << endl;

    return 0;
}
