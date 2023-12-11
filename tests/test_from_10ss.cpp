#include <bits/stdc++.h>
#include "big_frac.h"
using namespace std;
int main() {
    BigInt x;
    int y;
    cin >> x >> y;
    BigInt z = x.translate_from_10(y);
    z.print();
}