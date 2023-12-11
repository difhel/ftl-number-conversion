#include <bits/stdc++.h>
#include "big_frac.h"
using namespace std;
int main() {
    BigInt x;
    cin >> x;
    BigInt z = x.translate_to_10();
    z.print();
}