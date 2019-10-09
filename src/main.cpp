#include <iostream>

#include "babygiant.h"

using namespace std;
using namespace elliptic;

int main() {

    Curve curve = Curve(0, 7, 37);
    Point p = Point(6, 1);
    Point q = Point(8, 1);

    cout << p << " + " << q << " = " << curve.add(p, q) << endl;

    return 0;
}
