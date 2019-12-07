#include <iostream>
#include "approximatemul/bbm.h"
#include "approximatemul/bm07.h"
#include "approximatemul/pebm.h"

using namespace std;


int main() {
    int x = 1111, y = 1013;

    int result = bm07( x, y);
    cout << result << endl;
    result = bbm(x, y);
    cout<< result;
    return 0;
}